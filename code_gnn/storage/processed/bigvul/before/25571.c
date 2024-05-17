asmlinkage void do_exception_error(unsigned long ex, struct pt_regs *regs)
{
	show_excp_regs(__func__, -1, -1, regs);
	die_if_kernel("exception", regs, ex);
}
