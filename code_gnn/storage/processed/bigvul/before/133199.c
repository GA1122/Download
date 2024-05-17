base::TimeTicks SchedulerHelper::NextPendingDelayedTaskRunTime() const {
  CheckOnValidThread();
  return task_queue_manager_->NextPendingDelayedTaskRunTime();
}
