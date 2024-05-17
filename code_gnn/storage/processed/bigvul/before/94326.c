static long do_wait(struct wait_opts *wo)
{
	struct task_struct *tsk;
	int retval;

	trace_sched_process_wait(wo->wo_pid);

	init_waitqueue_func_entry(&wo->child_wait, child_wait_callback);
	wo->child_wait.private = current;
	add_wait_queue(&current->signal->wait_chldexit, &wo->child_wait);
repeat:
	 
	wo->notask_error = -ECHILD;
	if ((wo->wo_type < PIDTYPE_MAX) &&
	   (!wo->wo_pid || hlist_empty(&wo->wo_pid->tasks[wo->wo_type])))
		goto notask;

	set_current_state(TASK_INTERRUPTIBLE);
	read_lock(&tasklist_lock);
	tsk = current;
	do {
		retval = do_wait_thread(wo, tsk);
		if (retval)
			goto end;

		retval = ptrace_do_wait(wo, tsk);
		if (retval)
			goto end;

		if (wo->wo_flags & __WNOTHREAD)
			break;
	} while_each_thread(current, tsk);
	read_unlock(&tasklist_lock);

notask:
	retval = wo->notask_error;
	if (!retval && !(wo->wo_flags & WNOHANG)) {
		retval = -ERESTARTSYS;
		if (!signal_pending(current)) {
			schedule();
			goto repeat;
		}
	}
end:
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&current->signal->wait_chldexit, &wo->child_wait);
	return retval;
}