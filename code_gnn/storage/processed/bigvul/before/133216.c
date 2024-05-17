void TaskQueue::EnqueueTaskLocked(const base::PendingTask& pending_task) {
  lock_.AssertAcquired();
  if (!task_queue_manager_)
    return;
  if (pump_policy_ == TaskQueueManager::PumpPolicy::AUTO &&
      incoming_queue_.empty())
    task_queue_manager_->MaybePostDoWorkOnMainRunner();
  incoming_queue_.push(pending_task);

  if (!pending_task.delayed_run_time.is_null()) {
    incoming_queue_.back().delayed_run_time = base::TimeTicks();
  }
  TraceQueueSize(true);
}
