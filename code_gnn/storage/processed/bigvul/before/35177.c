void exit_signals(struct task_struct *tsk)
{
	int group_stop = 0;
	struct task_struct *t;

	if (thread_group_empty(tsk) || signal_group_exit(tsk->signal)) {
		tsk->flags |= PF_EXITING;
		return;
	}

	spin_lock_irq(&tsk->sighand->siglock);
	 
	tsk->flags |= PF_EXITING;
	if (!signal_pending(tsk))
		goto out;

	 
	for (t = tsk; (t = next_thread(t)) != tsk; )
		if (!signal_pending(t) && !(t->flags & PF_EXITING))
			recalc_sigpending_and_wake(t);

	if (unlikely(tsk->signal->group_stop_count) &&
			!--tsk->signal->group_stop_count) {
		tsk->signal->flags = SIGNAL_STOP_STOPPED;
		group_stop = tracehook_notify_jctl(CLD_STOPPED, CLD_STOPPED);
	}
out:
	spin_unlock_irq(&tsk->sighand->siglock);

	if (unlikely(group_stop)) {
		read_lock(&tasklist_lock);
		do_notify_parent_cldstop(tsk, group_stop);
		read_unlock(&tasklist_lock);
	}
}
