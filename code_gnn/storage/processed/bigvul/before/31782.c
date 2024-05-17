static int ptrace_signal(int signr, siginfo_t *info)
{
	ptrace_signal_deliver();
	 
	current->jobctl |= JOBCTL_STOP_DEQUEUED;
	ptrace_stop(signr, CLD_TRAPPED, 0, info);

	 
	signr = current->exit_code;
	if (signr == 0)
		return signr;

	current->exit_code = 0;

	 
	if (signr != info->si_signo) {
		info->si_signo = signr;
		info->si_errno = 0;
		info->si_code = SI_USER;
		rcu_read_lock();
		info->si_pid = task_pid_vnr(current->parent);
		info->si_uid = from_kuid_munged(current_user_ns(),
						task_uid(current->parent));
		rcu_read_unlock();
	}

	 
	if (sigismember(&current->blocked, signr)) {
		specific_send_sig_info(signr, info, current);
		signr = 0;
	}

	return signr;
}
