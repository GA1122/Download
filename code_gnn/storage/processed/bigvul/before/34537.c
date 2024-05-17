void set_task_comm(struct task_struct *tsk, char *buf)
{
	task_lock(tsk);

	trace_task_rename(tsk, buf);

	 
	memset(tsk->comm, 0, TASK_COMM_LEN);
	wmb();
	strlcpy(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
	perf_event_comm(tsk);
}
