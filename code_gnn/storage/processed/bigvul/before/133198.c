bool SchedulerHelper::IsQueueEnabled(size_t queue_index) const {
  CheckOnValidThread();
  return task_queue_selector_->IsQueueEnabled(queue_index);
}
