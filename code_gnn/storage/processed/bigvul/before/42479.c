void md_unregister_thread(struct md_thread **threadp)
{
	struct md_thread *thread = *threadp;
	if (!thread)
		return;
	pr_debug("interrupting MD-thread pid %d\n", task_pid_nr(thread->tsk));
	 
	spin_lock(&pers_lock);
	*threadp = NULL;
	spin_unlock(&pers_lock);

	kthread_stop(thread->tsk);
	kfree(thread);
}