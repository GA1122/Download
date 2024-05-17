static long do_wait(enum pid_type type, struct pid *pid, int options,
		    struct siginfo __user *infop, int __user *stat_addr,
		    struct rusage __user *ru)
{
	DECLARE_WAITQUEUE(wait, current);
	struct task_struct *tsk;
	int retval;

	trace_sched_process_wait(pid);

	add_wait_queue(&current->signal->wait_chldexit,&wait);
repeat:
	 
	retval = -ECHILD;
	if ((type < PIDTYPE_MAX) && (!pid || hlist_empty(&pid->tasks[type])))
		goto end;

	current->state = TASK_INTERRUPTIBLE;
	read_lock(&tasklist_lock);
	tsk = current;
	do {
		int tsk_result = do_wait_thread(tsk, &retval,
						type, pid, options,
						infop, stat_addr, ru);
		if (!tsk_result)
			tsk_result = ptrace_do_wait(tsk, &retval,
						    type, pid, options,
						    infop, stat_addr, ru);
		if (tsk_result) {
			 
			retval = tsk_result;
			goto end;
		}

		if (options & __WNOTHREAD)
			break;
		tsk = next_thread(tsk);
		BUG_ON(tsk->signal != current->signal);
	} while (tsk != current);
	read_unlock(&tasklist_lock);

	if (!retval && !(options & WNOHANG)) {
		retval = -ERESTARTSYS;
		if (!signal_pending(current)) {
			schedule();
			goto repeat;
		}
	}

end:
	current->state = TASK_RUNNING;
	remove_wait_queue(&current->signal->wait_chldexit,&wait);
	if (infop) {
		if (retval > 0)
			retval = 0;
		else {
			 
			if (!retval)
				retval = put_user(0, &infop->si_signo);
			if (!retval)
				retval = put_user(0, &infop->si_errno);
			if (!retval)
				retval = put_user(0, &infop->si_code);
			if (!retval)
				retval = put_user(0, &infop->si_pid);
			if (!retval)
				retval = put_user(0, &infop->si_uid);
			if (!retval)
				retval = put_user(0, &infop->si_status);
		}
	}
	return retval;
}
