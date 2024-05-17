static int do_signal_stop(int signr)
{
	struct signal_struct *sig = current->signal;
	int notify;

	if (!sig->group_stop_count) {
		struct task_struct *t;

		if (!likely(sig->flags & SIGNAL_STOP_DEQUEUED) ||
		    unlikely(signal_group_exit(sig)))
			return 0;
		 
		sig->group_exit_code = signr;

		sig->group_stop_count = 1;
		for (t = next_thread(current); t != current; t = next_thread(t))
			 
			if (!(t->flags & PF_EXITING) &&
			    !task_is_stopped_or_traced(t)) {
				sig->group_stop_count++;
				signal_wake_up(t, 0);
			}
	}
	 
	notify = sig->group_stop_count == 1 ? CLD_STOPPED : 0;
	notify = tracehook_notify_jctl(notify, CLD_STOPPED);
	 
	if (sig->group_stop_count) {
		if (!--sig->group_stop_count)
			sig->flags = SIGNAL_STOP_STOPPED;
		current->exit_code = sig->group_exit_code;
		__set_current_state(TASK_STOPPED);
	}
	spin_unlock_irq(&current->sighand->siglock);

	if (notify) {
		read_lock(&tasklist_lock);
		do_notify_parent_cldstop(current, notify);
		read_unlock(&tasklist_lock);
	}

	 
	do {
		schedule();
	} while (try_to_freeze());

	tracehook_finish_jctl();
	current->exit_code = 0;

	return 1;
}
