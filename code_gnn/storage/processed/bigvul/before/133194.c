TaskQueueManager::QueueState SchedulerHelper::GetQueueState(size_t queue_index)
    const {
  CheckOnValidThread();
  return task_queue_manager_->GetQueueState(queue_index);
}
