__releases(lock)
{
	DEFINE_WAIT(wait);

	prepare_to_wait(q, &wait, TASK_INTERRUPTIBLE);
	spin_unlock_irqrestore(lock, flags);
	timeout = schedule_timeout(timeout);
	finish_wait(q, &wait);

	return timeout;
}
