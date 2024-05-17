bool TaskQueue::NextPendingDelayedTaskRunTime(
    base::TimeTicks* next_pending_delayed_task) {
  base::AutoLock lock(lock_);
  if (delayed_task_queue_.empty())
    return false;
  *next_pending_delayed_task = delayed_task_queue_.top().delayed_run_time;
  return true;
}
