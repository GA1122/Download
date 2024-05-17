static int prepare_signal(int sig, struct task_struct *p, int from_ancestor_ns)
{
	struct signal_struct *signal = p->signal;
	struct task_struct *t;

	if (unlikely(signal->flags & SIGNAL_GROUP_EXIT)) {
		 
	} else if (sig_kernel_stop(sig)) {
		 
		rm_from_queue(sigmask(SIGCONT), &signal->shared_pending);
		t = p;
		do {
			rm_from_queue(sigmask(SIGCONT), &t->pending);
		} while_each_thread(p, t);
	} else if (sig == SIGCONT) {
		unsigned int why;
		 
		rm_from_queue(SIG_KERNEL_STOP_MASK, &signal->shared_pending);
		t = p;
		do {
			unsigned int state;
			rm_from_queue(SIG_KERNEL_STOP_MASK, &t->pending);
			 
			state = __TASK_STOPPED;
			if (sig_user_defined(t, SIGCONT) && !sigismember(&t->blocked, SIGCONT)) {
				set_tsk_thread_flag(t, TIF_SIGPENDING);
				state |= TASK_INTERRUPTIBLE;
			}
			wake_up_state(t, state);
		} while_each_thread(p, t);

		 
		why = 0;
		if (signal->flags & SIGNAL_STOP_STOPPED)
			why |= SIGNAL_CLD_CONTINUED;
		else if (signal->group_stop_count)
			why |= SIGNAL_CLD_STOPPED;

		if (why) {
			 
			signal->flags = why | SIGNAL_STOP_CONTINUED;
			signal->group_stop_count = 0;
			signal->group_exit_code = 0;
		} else {
			 
			signal->flags &= ~SIGNAL_STOP_DEQUEUED;
		}
	}

	return !sig_ignored(p, sig, from_ancestor_ns);
}
