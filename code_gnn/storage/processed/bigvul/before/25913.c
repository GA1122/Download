void send_sigtrap(struct task_struct *tsk, struct pt_regs *regs,
					 int error_code, int si_code)
{
	struct siginfo info;

	fill_sigtrap_info(tsk, regs, error_code, si_code, &info);
	 
	force_sig_info(SIGTRAP, &info, tsk);
}