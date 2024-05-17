static struct taskstats *taskstats_tgid_alloc(struct task_struct *tsk)
{
	struct signal_struct *sig = tsk->signal;
	struct taskstats *stats;

	if (sig->stats || thread_group_empty(tsk))
		goto ret;

	 
	stats = kmem_cache_zalloc(taskstats_cache, GFP_KERNEL);

	spin_lock_irq(&tsk->sighand->siglock);
	if (!sig->stats) {
		sig->stats = stats;
		stats = NULL;
	}
	spin_unlock_irq(&tsk->sighand->siglock);

	if (stats)
		kmem_cache_free(taskstats_cache, stats);
ret:
	return sig->stats;
}