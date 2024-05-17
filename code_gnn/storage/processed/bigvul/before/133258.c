const char* TaskQueueManager::WakeupPolicyToString(
    TaskQueueManager::WakeupPolicy wakeup_policy) {
  switch (wakeup_policy) {
    case TaskQueueManager::WakeupPolicy::CAN_WAKE_OTHER_QUEUES:
      return "can_wake_other_queues";
    case TaskQueueManager::WakeupPolicy::DONT_WAKE_OTHER_QUEUES:
      return "dont_wake_other_queues";
    default:
      NOTREACHED();
      return nullptr;
  }
}
