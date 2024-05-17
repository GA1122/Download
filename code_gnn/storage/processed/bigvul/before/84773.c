static int do_seccomp(struct pt_regs *regs)
{
	if (!test_thread_flag(TIF_SECCOMP))
		return 0;

	 
	regs->gpr[3] = -ENOSYS;

	 
	if (__secure_computing(NULL))
		return -1;

	 
	regs->gpr[3] = regs->orig_gpr3;

	return 0;
}
