static int wait_task_continued(struct task_struct *p, int options,
			       struct siginfo __user *infop,
			       int __user *stat_addr, struct rusage __user *ru)
{
	int retval;
	pid_t pid;
	uid_t uid;

	if (!unlikely(options & WCONTINUED))
		return 0;

	if (!(p->signal->flags & SIGNAL_STOP_CONTINUED))
		return 0;

	spin_lock_irq(&p->sighand->siglock);
	 
	if (!(p->signal->flags & SIGNAL_STOP_CONTINUED)) {
		spin_unlock_irq(&p->sighand->siglock);
		return 0;
	}
	if (!unlikely(options & WNOWAIT))
		p->signal->flags &= ~SIGNAL_STOP_CONTINUED;
	spin_unlock_irq(&p->sighand->siglock);

	pid = task_pid_vnr(p);
	uid = p->uid;
	get_task_struct(p);
	read_unlock(&tasklist_lock);

	if (!infop) {
		retval = ru ? getrusage(p, RUSAGE_BOTH, ru) : 0;
		put_task_struct(p);
		if (!retval && stat_addr)
			retval = put_user(0xffff, stat_addr);
		if (!retval)
			retval = pid;
	} else {
		retval = wait_noreap_copyout(p, pid, uid,
					     CLD_CONTINUED, SIGCONT,
					     infop, ru);
		BUG_ON(retval == 0);
	}

	return retval;
}