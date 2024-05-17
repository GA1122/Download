void SchedulerHelper::SetQueueName(size_t queue_index, const char* name) {
  CheckOnValidThread();
  task_queue_manager_->SetQueueName(queue_index, name);
}
