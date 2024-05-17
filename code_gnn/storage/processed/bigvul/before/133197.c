bool SchedulerHelper::IsQueueEmpty(size_t queue_index) const {
  CheckOnValidThread();
  return task_queue_manager_->IsQueueEmpty(queue_index);
}
