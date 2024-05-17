down_write_failed(struct ld_semaphore *sem, long count, long timeout)
{
	struct ldsem_waiter waiter;
	struct task_struct *tsk = current;
	long adjust = -LDSEM_ACTIVE_BIAS;
	int locked = 0;

	 
	raw_spin_lock_irq(&sem->wait_lock);

	 
	do {
		if (ldsem_cmpxchg(&count, count + adjust, sem))
			break;
		if ((count & LDSEM_ACTIVE_MASK) == LDSEM_ACTIVE_BIAS) {
			raw_spin_unlock_irq(&sem->wait_lock);
			return sem;
		}
	} while (1);

	list_add_tail(&waiter.list, &sem->write_wait);

	waiter.task = tsk;

	set_task_state(tsk, TASK_UNINTERRUPTIBLE);
	for (;;) {
		if (!timeout)
			break;
		raw_spin_unlock_irq(&sem->wait_lock);
		timeout = schedule_timeout(timeout);
		raw_spin_lock_irq(&sem->wait_lock);
		set_task_state(tsk, TASK_UNINTERRUPTIBLE);
		if ((locked = writer_trylock(sem)))
			break;
	}

	if (!locked)
		ldsem_atomic_update(-LDSEM_WAIT_BIAS, sem);
	list_del(&waiter.list);
	raw_spin_unlock_irq(&sem->wait_lock);

	__set_task_state(tsk, TASK_RUNNING);

	 
	if (!locked)
		return NULL;
	return sem;
}
