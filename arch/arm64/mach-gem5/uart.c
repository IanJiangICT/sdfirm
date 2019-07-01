#include <target/generic.h>
#include <target/uart.h>

#ifdef CONFIG_CONSOLE_OUTPUT
void uart_hw_con_write(uint8_t byte)
{
	pl01x_write_byte(byte);
}
#endif

#ifdef CONFIG_CONSOLE_INPUT
bool uart_hw_con_poll(void)
{
	return pl01x_read_poll();
}

uint8_t uart_hw_con_read(void)
{
	return pl01x_read_byte();
}
#endif

void uart_hw_irq_init(void)
{
	pl01x_irq_init();
}

void uart_hw_irq_ack(void)
{
	pl01x_irq_ack();
}

void uart_hw_con_init(void)
{
	pl01x_con_init();
}

#ifdef CONFIG_MMU
caddr_t uart_hw_reg_base[UART_HW_MAX_PORTS] = {
	__UART_BASE(0),
	__UART_BASE(1),
	__UART_BASE(2),
	__UART_BASE(3),
};

void uart_hw_mmu_init(void)
{
	set_fixmap_io(FIX_UART0, __UART_BASE(0) & PAGE_MASK);
	uart_hw_reg_base[0] = fix_to_virt(FIX_UART0);
	uart_hw_con_init();
	printf("FIXMAP: UART0: %016llx -> %016llx\n",
	       __UART_BASE(0), fix_to_virt(FIX_UART0));
	printf("UART_REG0: %016llx\n", UART_REG(0, 0x000));
	printf("UART_REG0: %016llx\n", UART_REG(0, 0x000));
}
#endif
