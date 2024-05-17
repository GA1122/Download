static int wait_task_stopped(int ptrace, struct task_struct *p,
			     int options, struct siginfo __user *infop,
			     int __user *stat_addr, struct rusage __user *ru)
{
	int retval, exit_code, why;
	uid_t uid = 0;  
	pid_t pid;

	if (!(options & WUNTRACED))
		return 0;

	exit_code = 0;
	spin_lock_irq(&p->sighand->siglock);

	if (unlikely(!task_is_stopped_or_traced(p)))
		goto unlock_sig;

	if (!ptrace && p->signal->group_stop_count > 0)
		 
		goto unlock_sig;

	exit_code = p->exit_code;
	if (!exit_code)
		goto unlock_sig;

	if (!unlikely(options & WNOWAIT))
		p->exit_code = 0;

	uid = p->uid;
unlock_sig:
	spin_unlock_irq(&p->sighand->siglock);
	if (!exit_code)
		return 0;

	 
	get_task_struct(p);
	pid = task_pid_vnr(p);
	why = ptrace ? CLD_TRAPPED : CLD_STOPPED;
	read_unlock(&tasklist_lock);

	if (unlikely(options & WNOWAIT))
		return wait_noreap_copyout(p, pid, uid,
					   why, exit_code,
					   infop, ru);

	retval = ru ? getrusage(p, RUSAGE_BOTH, ru) : 0;
	if (!retval && stat_addr)
		retval = put_user((exit_code << 8) | 0x7f, stat_addr);
	if (!retval && infop)
		retval = put_user(SIGCHLD, &infop->si_signo);
	if (!retval && infop)
		retval = put_user(0, &infop->si_errno);
	if (!retval && infop)
		retval = put_user((short)why, &infop->si_code);
	if (!retval && infop)
		retval = put_user(exit_code, &infop->si_status);
	if (!retval && infop)
		retval = put_user(pid, &infop->si_pid);
	if (!retval && infop)
		retval = put_user(uid, &infop->si_uid);
	if (!retval)
		retval = pid;
	put_task_struct(p);

	BUG_ON(!retval);
	return retval;
}