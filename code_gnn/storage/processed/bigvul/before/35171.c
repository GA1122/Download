int do_notify_parent(struct task_struct *tsk, int sig)
{
	struct siginfo info;
	unsigned long flags;
	struct sighand_struct *psig;
	int ret = sig;

	BUG_ON(sig == -1);

 	 
 	BUG_ON(task_is_stopped_or_traced(tsk));

	BUG_ON(!task_ptrace(tsk) &&
	       (tsk->group_leader != tsk || !thread_group_empty(tsk)));

	info.si_signo = sig;
	info.si_errno = 0;
	 
	rcu_read_lock();
	info.si_pid = task_pid_nr_ns(tsk, tsk->parent->nsproxy->pid_ns);
	info.si_uid = __task_cred(tsk)->uid;
	rcu_read_unlock();

	info.si_utime = cputime_to_clock_t(cputime_add(tsk->utime,
				tsk->signal->utime));
	info.si_stime = cputime_to_clock_t(cputime_add(tsk->stime,
				tsk->signal->stime));

	info.si_status = tsk->exit_code & 0x7f;
	if (tsk->exit_code & 0x80)
		info.si_code = CLD_DUMPED;
	else if (tsk->exit_code & 0x7f)
		info.si_code = CLD_KILLED;
	else {
		info.si_code = CLD_EXITED;
		info.si_status = tsk->exit_code >> 8;
	}

	psig = tsk->parent->sighand;
	spin_lock_irqsave(&psig->siglock, flags);
	if (!task_ptrace(tsk) && sig == SIGCHLD &&
	    (psig->action[SIGCHLD-1].sa.sa_handler == SIG_IGN ||
	     (psig->action[SIGCHLD-1].sa.sa_flags & SA_NOCLDWAIT))) {
		 
		ret = tsk->exit_signal = -1;
		if (psig->action[SIGCHLD-1].sa.sa_handler == SIG_IGN)
			sig = -1;
	}
	if (valid_signal(sig) && sig > 0)
		__group_send_sig_info(sig, &info, tsk->parent);
	__wake_up_parent(tsk, tsk->parent);
	spin_unlock_irqrestore(&psig->siglock, flags);

	return ret;
}
