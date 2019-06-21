#include <stdio.h>
#include <string.h>
#include <target/paging.h>
#include <target/arch.h>
#include <target/linkage.h>
#include <target/compiler.h>

static phys_addr_t early_pgtable_alloc(int shift)
{
	phys_addr_t phys;
	void *ptr;

	phys = page_alloc();
	if (!phys) {
		printf("Failed to allocate page table page\n");
		BUG();
	}

	/* The FIX_{PGD,PUD,PMD} slots may be in active use, but the
	 * FIX_PTE slot will be free, so we can (ab)use the FIX_PTE slot to
	 * initialise any level of table.
	 */
	ptr = pte_set_fixmap(phys);
	printf("ALLOC: %016llx\n", ptr);
	memory_set((caddr_t)ptr, 0, PAGE_SIZE);

	/* Implicit barriers also ensure the zeroed page is visible to the
	 * page table walker.
	 */
	pte_clear_fixmap();
	return phys;
}

static void alloc_init_pte(pmd_t *pmdp, caddr_t addr,
			   caddr_t end, phys_addr_t phys,
			   pgprot_t prot,
			   phys_addr_t (*pgtable_alloc)(int))
{
	caddr_t next;
	pmd_t pmd = READ_ONCE(*pmdp);
	pte_t *ptep;

	BUG_ON(pmd_sect(pmd));
	if (pmd_none(pmd)) {
		phys_addr_t pte_phys;
		BUG_ON(!pgtable_alloc);
		pte_phys = pgtable_alloc(PAGE_SHIFT);
		__pmd_populate(pmdp, pte_phys, PMD_TYPE_TABLE);
		pmd = READ_ONCE(*pmdp);
	}
	BUG_ON(pmd_bad(pmd));

	ptep = pte_set_fixmap_offset(pmdp, addr);
	do {
		next = pte_addr_end(addr, end);
		set_pte(ptep, pfn_pte(phys_to_pfn(phys), prot));
		phys += next - addr;
	} while (addr = next, addr != end);
	pte_clear_fixmap();
}

static void alloc_init_pmd(pud_t *pudp, caddr_t addr,
			   caddr_t end, phys_addr_t phys,
			   pgprot_t prot,
			   phys_addr_t (*pgtable_alloc)(int))
{
	caddr_t next;
	pud_t pud = READ_ONCE(*pudp);
	pmd_t *pmdp;

	/* Check for initial section mappings in the pgd/pud. */
	BUG_ON(pud_sect(pud));
	if (pud_none(pud)) {
		phys_addr_t pmd_phys;
		BUG_ON(!pgtable_alloc);
		pmd_phys = pgtable_alloc(PMD_SHIFT);
		__pud_populate(pudp, pmd_phys, PUD_TYPE_TABLE);
		pud = READ_ONCE(*pudp);
	}
	BUG_ON(pud_bad(pud));

	pmdp = pmd_set_fixmap_offset(pudp, addr);
	printf("PMD: %016llx\n", pmdp);
	do {
		next = pmd_addr_end(addr, end);
		alloc_init_pte(pmdp, addr, next, phys,
			       prot, pgtable_alloc);
		BUG_ON(pmd_val(old_pmd) != 0 &&
		       pmd_val(old_pmd) != pmd_val(*pmd));
		phys += next - addr;
	} while (addr = next, addr != end);
}

static void alloc_init_pud(pgd_t *pgdp, caddr_t addr, caddr_t end,
			   phys_addr_t phys, pgprot_t prot,
			   phys_addr_t (*pgtable_alloc)(int))
{
	caddr_t next;
	pud_t *pudp;
	pgd_t pgd = READ_ONCE(*pgdp);

	if (pgd_none(pgd)) {
		phys_addr_t pud_phys;
		BUG_ON(!pgtable_alloc);
		pud_phys = pgtable_alloc(PUD_SHIFT);
		__pgd_populate(pgdp, pud_phys, PUD_TYPE_TABLE);
		pgd = READ_ONCE(*pgdp);
	}
	BUG_ON(pgd_bad(pgd));

	pudp = pud_set_fixmap_offset(pgdp, addr);
	printf("PUD: %016llx\n", pudp);
	do {
		next = pud_addr_end(addr, end);
		alloc_init_pmd(pudp, addr, next, phys, prot,
			       pgtable_alloc);
		BUG_ON(pud_val(old_pud) != 0 &&
			       pud_val(old_pud) != READ_ONCE(pud_val(*pudp)));
		phys += next - addr;
	} while (pudp++, addr = next, addr != end);
	pud_clear_fixmap();
}

static void __create_pgd_mapping(pgd_t *pgdir, phys_addr_t phys,
				 caddr_t virt, phys_addr_t size,
				 pgprot_t prot,
				 phys_addr_t (*pgtable_alloc)(int))
{
	caddr_t addr, length, end, next;
	pgd_t *pgdp = pgd_offset_raw(pgdir, virt);

	printf("MAP: phys: %016llx\n", phys);
	printf("MAP: virt: %016llx\n", virt);
	printf("MAP: size: %016llx\n", size);

	/* If the virtual and physical address don't have the same offset
	 * within a page, we cannot map the region as the caller expects.
	 */
	BUG_ON((phys ^ virt) & ~PAGE_MASK);

	phys &= PAGE_MASK;
	addr = virt & PAGE_MASK;
	length = PAGE_ALIGN(size + (virt & ~PAGE_MASK));
	end = addr + length;
	do {
		next = pgd_addr_end(addr, end);
		alloc_init_pud(pgdp, addr, next, phys, prot, pgtable_alloc);
		phys += next - addr;
	} while (pgdp++, addr = next, addr != end);
}

static void map_kernel_segment(pgd_t *pgdp, void *va_start, void *va_end,
			       pgprot_t prot)
{
	phys_addr_t pa_start = __pa_symbol(va_start);
	phys_addr_t size = va_end - va_start;

	BUG_ON(!PAGE_ALIGNED(pa_start));
	BUG_ON(!PAGE_ALIGNED(size));

	__create_pgd_mapping(pgdp, pa_start, (caddr_t)va_start, size, prot,
			     early_pgtable_alloc);
}

static void map_kernel(pgd_t *pgdp)
{
	/* External debuggers may need to write directly to the text
	 * mapping to install SW breakpoints. Allow this (only) when
	 * explicitly requested with rodata=off.
	 */
#ifdef CONFIG_MMU_TEXT_MODIFY
#define PAGE_TEXT_PROT		PAGE_KERNEL_EXEC
#else
#define PAGE_TEXT_PROT		PAGE_KERNEL_ROX
#endif

	/* Only rodata will be remapped with different permissions later on,
	 * all other segments are allowed to use contiguous mappings.
	 */
	map_kernel_segment(pgdp, __stext, __etext, PAGE_TEXT_PROT);
	map_kernel_segment(pgdp, __start_rodata, __end_rodata, PAGE_KERNEL);
	map_kernel_segment(pgdp, __sdata, __edata, PAGE_KERNEL);

	/* The fixmap falls in a separate pgd to the kernel, and doesn't live
	 * in the carveout for the mmu_pg_dir. We can simply re-use the
	 * existing dir for the fixmap.
	 */
	set_pgd(pgd_offset_raw(pgdp, FIXADDR_START),
		READ_ONCE(*pgd_offset(FIXADDR_START)));
}

static pte_t bm_pte[PTRS_PER_PTE] __page_aligned_bss;
#if PGTABLE_LEVELS > 2
static pmd_t bm_pmd[PTRS_PER_PMD] __page_aligned_bss;
#endif
#if PGTABLE_LEVES > 3
static pud_t bm_pud[PTRS_PER_PUD] __page_aligned_bss;
#endif

static inline pud_t *fixmap_pud(caddr_t addr)
{
	pgd_t *pgdp = pgd_offset(addr);
#ifdef CONFIG_DEBUG_PANIC
	pgd_t pgd = READ_ONCE(*pgdp);
#endif

	BUG_ON(pgd_none(pgd) || pgd_bad(pgd));

	return pud_offset(pgdp, addr);
}

static inline pmd_t *fixmap_pmd(caddr_t addr)
{
	pud_t *pudp = fixmap_pud(addr);
#ifdef CONFIG_DEBUG_PANIC
	pud_t pud = READ_ONCE(*pudp);
#endif

	BUG_ON(pud_none(pud) || pud_bad(pud));

	return pmd_offset(pudp, addr);
}

static inline pte_t *fixmap_pte(caddr_t addr)
{
#if 1
	return &bm_pte[pte_index(addr)];
#else
	pmd_t *pmdp = fixmap_pmd(addr);

	BUG_ON(pmd_none(*pmdp) || pmd_bad(*pmdp));
	return pte_offset_kernel(pmdp, addr);
#endif
}

/* Unusually, this is also called in IRQ context (ghes_iounmap_irq) so if we
 * ever need to use IPIs for TLB broadcasting, then we're in trouble here.
 */
void __set_fixmap(enum fixed_addresses idx,
		  phys_addr_t phys, pgprot_t flags)
{
	caddr_t addr = __fix_to_virt(idx);
	pte_t *ptep;

	BUG_ON(idx <= FIX_HOLE || idx >= __end_of_fixed_addresses);

	ptep = fixmap_pte(addr);

	if (pgprot_val(flags)) {
		set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT, flags));
	} else {
		pte_clear(addr, ptep);
		/* flush_tlb_kernel_range(addr, addr+PAGE_SIZE); */
	}
}

void early_fixmap_init(void)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	caddr_t addr = FIXADDR_START;

	printf("FIX: %016llx - %016llx\n", FIXADDR_START, FIXADDR_END);
	printf("PGD: %016llx\n", mmu_pg_dir);
	pgd = pgd_offset(addr);
	pgd_populate(pgd, bm_pud);
	pud = pud_offset(pgd, addr);
	pud_populate(pud, bm_pmd);
	pmd = pmd_offset(pud, addr);
	pmd_populate_kernel(pmd, bm_pte);

	/* The boot-ioremap range spans multiple pmds, for which
	 * we are not preparted:
	 */
	BUG_ON((__fix_to_virt(FIX_BTMAP_BEGIN) >> PMD_SHIFT)
	       != (__fix_to_virt(FIX_BTMAP_END) >> PMD_SHIFT));

	if ((pmd != fixmap_pmd(fix_to_virt(FIX_BTMAP_BEGIN))) ||
	    pmd != fixmap_pmd(fix_to_virt(FIX_BTMAP_END))) {
		BUG();
		printf("pmd %p != %p, %p\n",
		       pmd, fixmap_pmd(fix_to_virt(FIX_BTMAP_BEGIN)),
		       fixmap_pmd(fix_to_virt(FIX_BTMAP_END)));
		printf("fix_to_virt(FIX_BTMAP_BEGIN): %08lx\n",
		       fix_to_virt(FIX_BTMAP_BEGIN));
		printf("fix_to_virt(FIX_BTMAP_END):   %08lx\n",
		       fix_to_virt(FIX_BTMAP_END));

		printf("FIX_BTMAP_END:       %d\n", FIX_BTMAP_END);
		printf("FIX_BTMAP_BEGIN:     %d\n", FIX_BTMAP_BEGIN);
	}
}

void paging_init(void)
{
        pgd_t *pgdp = pgd_set_fixmap(__pa_symbol(mmu_pg_dir));

	map_kernel(pgdp);
#if 0
	map_mem(pgdp);
#endif
	pgd_clear_fixmap();
#if 0
	cpu_replace_ttbr1(lm_alias(swapper_pg_dir));
	mmu_hw_ctrl_init();
#endif
}
