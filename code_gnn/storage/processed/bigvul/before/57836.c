down_read_failed(struct ld_semaphore *sem, long count, long timeout)
{
	struct ldsem_waiter waiter;
	struct task_struct *tsk = current;
	long adjust = -LDSEM_ACTIVE_BIAS + LDSEM_WAIT_BIAS;

	 
	raw_spin_lock_irq(&sem->wait_lock);

	 
	do {
		if (ldsem_cmpxchg(&count, count + adjust, sem))
			break;
		if (count > 0) {
			raw_spin_unlock_irq(&sem->wait_lock);
			return sem;
		}
	} while (1);

	list_add_tail(&waiter.list, &sem->read_wait);
	sem->wait_readers++;

	waiter.task = tsk;
	get_task_struct(tsk);

	 
	if ((count & LDSEM_ACTIVE_MASK) == 0)
		__ldsem_wake(sem);

	raw_spin_unlock_irq(&sem->wait_lock);

	 
	for (;;) {
		set_task_state(tsk, TASK_UNINTERRUPTIBLE);

		if (!waiter.task)
			break;
		if (!timeout)
			break;
		timeout = schedule_timeout(timeout);
	}

	__set_task_state(tsk, TASK_RUNNING);

	if (!timeout) {
		 
		raw_spin_lock_irq(&sem->wait_lock);
		if (waiter.task) {
			ldsem_atomic_update(-LDSEM_WAIT_BIAS, sem);
			list_del(&waiter.list);
			raw_spin_unlock_irq(&sem->wait_lock);
			put_task_struct(waiter.task);
			return NULL;
		}
		raw_spin_unlock_irq(&sem->wait_lock);
	}

	return sem;
}
