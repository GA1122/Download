bool TaskQueue::TaskIsOlderThanQueuedTasks(const base::PendingTask* task) {
  lock_.AssertAcquired();
  if (!task)
    return true;

  if (incoming_queue_.empty())
    return false;

  base::PendingTask oldest_queued_task = incoming_queue_.front();
  DCHECK(oldest_queued_task.delayed_run_time.is_null());
  DCHECK(task->delayed_run_time.is_null());

  return oldest_queued_task < *task;
}
