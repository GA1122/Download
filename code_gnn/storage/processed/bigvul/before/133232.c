const char* TaskQueueManager::PumpPolicyToString(
    TaskQueueManager::PumpPolicy pump_policy) {
  switch (pump_policy) {
    case TaskQueueManager::PumpPolicy::AUTO:
      return "auto";
    case TaskQueueManager::PumpPolicy::AFTER_WAKEUP:
      return "after_wakeup";
    case TaskQueueManager::PumpPolicy::MANUAL:
      return "manual";
    default:
      NOTREACHED();
      return nullptr;
  }
}
