bool TaskQueue::PostDelayedTaskImpl(const tracked_objects::Location& from_here,
                                    const base::Closure& task,
                                    base::TimeDelta delay,
                                    TaskType task_type) {
  base::AutoLock lock(lock_);
  if (!task_queue_manager_)
    return false;

  base::PendingTask pending_task(from_here, task, base::TimeTicks(),
                                 task_type != TaskType::NON_NESTABLE);
  task_queue_manager_->DidQueueTask(&pending_task);

  if (delay > base::TimeDelta()) {
    base::TimeTicks now = task_queue_manager_->Now();
    pending_task.delayed_run_time = now + delay;
    delayed_task_queue_.push(pending_task);
    TraceQueueSize(true);
    if (delayed_task_queue_.top().task.Equals(pending_task.task)) {
      LazyNow lazy_now(now);
      ScheduleDelayedWorkLocked(&lazy_now);
    }
    return true;
  }
  EnqueueTaskLocked(pending_task);
  return true;
}
