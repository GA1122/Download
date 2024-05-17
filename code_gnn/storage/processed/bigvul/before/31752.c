int do_sigtimedwait(const sigset_t *which, siginfo_t *info,
			const struct timespec *ts)
{
	struct task_struct *tsk = current;
	long timeout = MAX_SCHEDULE_TIMEOUT;
	sigset_t mask = *which;
	int sig;

	if (ts) {
		if (!timespec_valid(ts))
			return -EINVAL;
		timeout = timespec_to_jiffies(ts);
		 
		if (ts->tv_sec || ts->tv_nsec)
			timeout++;
	}

	 
	sigdelsetmask(&mask, sigmask(SIGKILL) | sigmask(SIGSTOP));
	signotset(&mask);

	spin_lock_irq(&tsk->sighand->siglock);
	sig = dequeue_signal(tsk, &mask, info);
	if (!sig && timeout) {
		 
		tsk->real_blocked = tsk->blocked;
		sigandsets(&tsk->blocked, &tsk->blocked, &mask);
		recalc_sigpending();
		spin_unlock_irq(&tsk->sighand->siglock);

		timeout = schedule_timeout_interruptible(timeout);

		spin_lock_irq(&tsk->sighand->siglock);
		__set_task_blocked(tsk, &tsk->real_blocked);
		siginitset(&tsk->real_blocked, 0);
		sig = dequeue_signal(tsk, &mask, info);
	}
	spin_unlock_irq(&tsk->sighand->siglock);

	if (sig)
		return sig;
	return timeout ? -EINTR : -EAGAIN;
}
