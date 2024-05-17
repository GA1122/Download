bool TaskQueue::ShouldAutoPumpQueueLocked(
    bool should_trigger_wakeup,
    const base::PendingTask* previous_task) {
  lock_.AssertAcquired();
  if (pump_policy_ == TaskQueueManager::PumpPolicy::MANUAL)
    return false;
  if (pump_policy_ == TaskQueueManager::PumpPolicy::AFTER_WAKEUP &&
      (!should_trigger_wakeup || TaskIsOlderThanQueuedTasks(previous_task)))
    return false;
  if (incoming_queue_.empty())
    return false;
  return true;
}
