interruptible_sleep_on_timeout(wait_queue_head_t *q, long timeout)
{
	return sleep_on_common(q, TASK_INTERRUPTIBLE, timeout);
}