static void fill_sigtrap_info(struct task_struct *tsk,
				struct pt_regs *regs,
				int error_code, int si_code,
				struct siginfo *info)
{
	tsk->thread.trap_no = 1;
	tsk->thread.error_code = error_code;

	memset(info, 0, sizeof(*info));
	info->si_signo = SIGTRAP;
	info->si_code = si_code;
	info->si_addr = user_mode_vm(regs) ? (void __user *)regs->ip : NULL;
}