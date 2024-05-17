static int ptrace_signal(int signr, siginfo_t *info,
			 struct pt_regs *regs, void *cookie)
{
	if (!task_ptrace(current))
		return signr;

	ptrace_signal_deliver(regs, cookie);

	 
	ptrace_stop(signr, 0, info);

	 
	signr = current->exit_code;
	if (signr == 0)
		return signr;

	current->exit_code = 0;

	 
	if (signr != info->si_signo) {
		info->si_signo = signr;
		info->si_errno = 0;
		info->si_code = SI_USER;
		info->si_pid = task_pid_vnr(current->parent);
		info->si_uid = task_uid(current->parent);
	}

	 
	if (sigismember(&current->blocked, signr)) {
		specific_send_sig_info(signr, info, current);
		signr = 0;
	}

	return signr;
}
