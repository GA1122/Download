static struct mm_struct *check_mem_permission(struct task_struct *task)
{
	struct mm_struct *mm;
	int err;

	 
	err = mutex_lock_killable(&task->signal->cred_guard_mutex);
	if (err)
		return ERR_PTR(err);

	mm = __check_mem_permission(task);
	mutex_unlock(&task->signal->cred_guard_mutex);

	return mm;
}
