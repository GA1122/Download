int dequeue_signal(struct task_struct *tsk, sigset_t *mask, siginfo_t *info)
{
	bool resched_timer = false;
	int signr;

	 
	signr = __dequeue_signal(&tsk->pending, mask, info, &resched_timer);
	if (!signr) {
		signr = __dequeue_signal(&tsk->signal->shared_pending,
					 mask, info, &resched_timer);
#ifdef CONFIG_POSIX_TIMERS
		 
		if (unlikely(signr == SIGALRM)) {
			struct hrtimer *tmr = &tsk->signal->real_timer;

			if (!hrtimer_is_queued(tmr) &&
			    tsk->signal->it_real_incr != 0) {
				hrtimer_forward(tmr, tmr->base->get_time(),
						tsk->signal->it_real_incr);
				hrtimer_restart(tmr);
			}
		}
#endif
	}

	recalc_sigpending();
	if (!signr)
		return 0;

	if (unlikely(sig_kernel_stop(signr))) {
		 
		current->jobctl |= JOBCTL_STOP_DEQUEUED;
	}
#ifdef CONFIG_POSIX_TIMERS
	if (resched_timer) {
		 
		spin_unlock(&tsk->sighand->siglock);
		posixtimer_rearm(info);
		spin_lock(&tsk->sighand->siglock);
	}
#endif
	return signr;
}
