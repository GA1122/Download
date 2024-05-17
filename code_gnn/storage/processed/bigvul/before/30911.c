char *get_task_comm(char *buf, struct task_struct *tsk)
{
	 
	task_lock(tsk);
	strncpy(buf, tsk->comm, sizeof(tsk->comm));
	task_unlock(tsk);
	return buf;
}
