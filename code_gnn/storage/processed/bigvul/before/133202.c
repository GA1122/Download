void SchedulerHelper::SetPumpPolicy(size_t queue_index,
                                    TaskQueueManager::PumpPolicy pump_policy) {
  CheckOnValidThread();
  return task_queue_manager_->SetPumpPolicy(queue_index, pump_policy);
}
