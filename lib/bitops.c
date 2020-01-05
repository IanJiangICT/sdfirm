#include <target/generic.h>
#include <target/atomic.h>

#ifdef CONFIG_BIT_HWEIGHT64
uint8_t hweight64(uint64_t quad)
{
	uint64_t res;

	res = quad - ((quad >> 1) & 0x5555555555555555ul);
	res = (res & 0x3333333333333333ul) +
	      ((res >> 2) & 0x3333333333333333ul);
	res = (res + (res >> 4)) & 0x0F0F0F0F0F0F0F0Ful;
	res = res + (res >> 8);
	res = res + (res >> 16);
	return (uint8_t)((res + (res >> 32)) & 0x00000000000000FFul);
}
#endif

uint8_t hweight16(uint16_t word)
{
	uint16_t res = word - ((word >> 1) & 0x5555);
	res = (res & 0x3333) + ((res >> 2) & 0x3333);
	res = (res + (res >> 4)) & 0x0F0F;
	return (res + (res >> 8)) & 0x00FF;
}

uint8_t hweight8(uint8_t byte)
{
	uint8_t res = byte - ((byte >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	return (res + (res >> 4)) & 0x0F;
}

uint8_t bitrev8(uint8_t byte)
{
	byte = ((byte & 0x55) << 1 | (byte & 0xAA) >> 1);
	byte = ((byte & 0x33) << 2 | (byte & 0xCC) >> 2);
	byte = ((byte & 0x0F) << 4 | (byte & 0xF0) >> 4);
	return byte;
}

/**
 * set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
void __set_bit(uint8_t nr, volatile bits_t *addr)
{
	bits_t mask = BITOP_MASK(nr);
	bits_t *p = ((bits_t *)addr) + BITOP_WORD(nr);
	*p |= mask;
}

void __clear_bit(uint8_t nr, volatile bits_t *addr)
{
	bits_t mask = BITOP_MASK(nr);
	bits_t *p = ((bits_t *)addr) + BITOP_WORD(nr);
	*p &= ~mask;
}

#ifdef CONFIG_SMP
void set_bit(uint8_t nr, volatile bits_t *addr)
{
	bits_t mask = BITOP_MASK(nr);
	bits_t *p = ((bits_t *)addr) + BITOP_WORD(nr);
	atomic_or(mask, (atomic_t *)p);
}

void clear_bit(uint8_t nr, volatile bits_t *addr)
{
	bits_t mask = BITOP_MASK(nr);
	bits_t *p = ((bits_t *)addr) + BITOP_WORD(nr);
	atomic_andnot(mask, (atomic_t *)p);
}
#endif

/**
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
boolean test_bit(uint8_t nr, const bits_t *addr)
{
	return (bits_t)1 & (addr[BITOP_WORD(nr)] >> (nr & (BITS_PER_UNIT-1)));
}

uint8_t find_next_set_bit(const bits_t *addr,
			  uint8_t size,
			  uint8_t offset)
{
	const bits_t *p = addr + BITOP_WORD(offset);
	uint8_t result = offset & ~(BITS_PER_UNIT-1);
	bits_t tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_UNIT;
	if (offset) {
		tmp = *(p++);
		tmp &= ((bits_t)(~((bits_t)0)) << offset);
		if (size < BITS_PER_UNIT)
			goto found_first;
		if ((bits_t)tmp)
			goto found_middle;
		size -= BITS_PER_UNIT;
		result += BITS_PER_UNIT;
	}
	while (size & ~(BITS_PER_UNIT-1)) {
		if ((bits_t)(tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_UNIT;
		size -= BITS_PER_UNIT;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp &= ((bits_t)(~((bits_t)0)) >> (BITS_PER_UNIT - size));
	if (tmp == (bits_t)0)
		return result + size;
found_middle:
	return result + __ffs8(tmp);
}

#ifdef CONFIG_BIT_FIND_CLEAR
uint8_t find_next_clear_bit(const bits_t *addr,
			    uint8_t size,
			    uint8_t offset)
{
	const bits_t *p = addr + BITOP_WORD(offset);
	uint8_t result = offset & ~(BITS_PER_UNIT-1);
	bits_t tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_UNIT;
	if (offset) {
		tmp = *(p++);
		tmp |= (bits_t)(~((bits_t)0)) >> (BITS_PER_UNIT - offset);
		if (size < BITS_PER_UNIT)
			goto found_first;
		if ((bits_t)(~tmp))
			goto found_middle;
		size -= BITS_PER_UNIT;
		result += BITS_PER_UNIT;
	}
	while (size & ~(BITS_PER_UNIT-1)) {
		if ((bits_t)(~(tmp = *(p++))))
			goto found_middle;
		result += BITS_PER_UNIT;
		size -= BITS_PER_UNIT;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp |= (bits_t)(~((bits_t)0)) << size;
	if (tmp == (bits_t)(~((bits_t)0)))	
		return result + size;
found_middle:
	return result + __ffz8(tmp);
}
#endif

#ifdef CONFIG_BIT_FLS8
uint8_t __fls8(uint8_t x)
{
	uint8_t r = 7;

	if (!x)
		return 0;
	if (!(x & (uint8_t)0xf0)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & (uint8_t)0xc0)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & (uint8_t)0x80)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}
#endif

uint8_t __ffs8(uint8_t byte)
{
	uint8_t num = 0;

	if (!byte)
		return 0;

	if ((byte & 0xf) == 0) {
		num += 4;
		byte >>= 4;
	}
	if ((byte & 0x3) == 0) {
		num += 2;
		byte >>= 2;
	}
	if ((byte & 0x1) == 0)
		num += 1;
	return num;
}

#ifdef CONFIG_BIT_FFS16
uint8_t __ffs16(uint16_t word)
{
	uint8_t num = 0;

	if (!word)
		return 0;

	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return num;
}
#endif

#ifdef CONFIG_BIT_FFS32
uint8_t __ffs32(uint32_t dword)
{
	uint8_t num = 0;

	if (!dword)
		return 0;

	if ((dword & 0xffff) == 0) {
		num += 16;
		dword >>= 16;
	}
	if ((dword & 0xff) == 0) {
		num += 8;
		dword >>= 8;
	}
	if ((dword & 0xf) == 0) {
		num += 4;
		dword >>= 4;
	}
	if ((dword & 0x3) == 0) {
		num += 2;
		dword >>= 2;
	}
	if ((dword & 0x1) == 0)
		num += 1;
	return num;
}
#endif

#ifdef CONFIG_BIT_FLS16
uint8_t __fls16(uint16_t word)
{
	int num = 16 - 1;

	if (!word)
		return 0;
	if (!(word & 0xff00u)) {
		word <<= 8;
		num -= 8;
	}
	if (!(word & 0xf000u)) {
		word <<= 4;
		num -= 4;
	}
	if (!(word & 0xc000u)) {
		word <<= 2;
		num -= 2;
	}
	if (!(word & 0x8000u)) {
		word <<= 1;
		num -= 1;
	}
	return num;
}
#endif

#ifdef CONFIG_BIT_FLS32
uint8_t __fls32(uint32_t dword)
{
	int num = 32 - 1;

	if (!dword)
		return 0;
	if (!(dword & UL(0xffff0000))) {
		dword <<= 16;
		num -= 16;
	}
	if (!(dword & UL(0xff000000))) {
		dword <<= 8;
		num -= 8;
	}
	if (!(dword & UL(0xf0000000))) {
		dword <<= 4;
		num -= 4;
	}
	if (!(dword & UL(0xc0000000))) {
		dword <<= 2;
		num -= 2;
	}
	if (!(dword & UL(0x80000000))) {
		dword <<= 1;
		num -= 1;
	}
	return num;
}
#endif

#ifdef CONFIG_BIT_FLS64
uint8_t __fls64(uint64_t quad)
{
	int num = 64 - 1;

	if (!quad)
		return 0;
	if (!(quad & ULL(0xffffffff00000000))) {
		quad <<= 32;
		num -= 32;
	}
	if (!(quad & ULL(0xffff000000000000))) {
		quad <<= 16;
		num -= 16;
	}
	if (!(quad & ULL(0xff00000000000000))) {
		quad <<= 8;
		num -= 8;
	}
	if (!(quad & ULL(0xf000000000000000))) {
		quad <<= 4;
		num -= 4;
	}
	if (!(quad & ULL(0xc000000000000000))) {
		quad <<= 2;
		num -= 2;
	}
	if (!(quad & ULL(0x8000000000000000))) {
		quad <<= 1;
		num -= 1;
	}
	return num;
}
#endif
