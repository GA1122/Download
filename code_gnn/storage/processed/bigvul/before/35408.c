dotraplinkage void do_coprocessor_error(struct pt_regs *regs, long error_code)
{
	enum ctx_state prev_state;

	prev_state = exception_enter();
	math_error(regs, error_code, X86_TRAP_MF);
	exception_exit(prev_state);
}