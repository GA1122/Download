__weak void board_init_f_init_stack_protection(void)
{
#if CONFIG_IS_ENABLED(SYS_REPORT_STACK_F_USAGE)
	ulong stack_bottom = gd->start_addr_sp -
		CONFIG_VAL(SIZE_LIMIT_PROVIDE_STACK);

	 
	memset((void *)stack_bottom, CONFIG_VAL(SYS_STACK_F_CHECK_BYTE),
	       CONFIG_VAL(SIZE_LIMIT_PROVIDE_STACK) - 0x20);
#endif
}
