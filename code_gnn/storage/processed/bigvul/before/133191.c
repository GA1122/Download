void SchedulerHelper::DisableQueue(size_t queue_index) {
  CheckOnValidThread();
  task_queue_selector_->DisableQueue(queue_index);
}
