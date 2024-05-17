void TaskQueue::SetPumpPolicy(TaskQueueManager::PumpPolicy pump_policy) {
  base::AutoLock lock(lock_);
  if (pump_policy == TaskQueueManager::PumpPolicy::AUTO &&
      pump_policy_ != TaskQueueManager::PumpPolicy::AUTO) {
    PumpQueueLocked();
  }
  pump_policy_ = pump_policy;
}
