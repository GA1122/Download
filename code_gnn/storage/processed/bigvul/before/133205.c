void SchedulerHelper::SetWakeupPolicy(
    size_t queue_index,
    TaskQueueManager::WakeupPolicy wakeup_policy) {
  CheckOnValidThread();
  return task_queue_manager_->SetWakeupPolicy(queue_index, wakeup_policy);
}
