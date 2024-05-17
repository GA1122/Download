void NetworkThrottleManagerImpl::OnThrottlePriorityChanged(
    NetworkThrottleManagerImpl::ThrottleImpl* throttle,
    RequestPriority old_priority,
    RequestPriority new_priority) {
  if (throttle->IsBlocked() && new_priority != THROTTLED) {
    UnblockThrottle(throttle);
  }
}
