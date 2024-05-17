void dump_stack(void)
{
	struct pt_regs regs;

	prepare_frametrace(&regs);
	show_backtrace(current, &regs);
}
