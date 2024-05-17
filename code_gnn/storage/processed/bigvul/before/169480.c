void NetworkThrottleManagerImpl::MaybeUnblockThrottles() {
  RecomputeOutstanding();

  while (outstanding_throttles_.size() < kActiveRequestThrottlingLimit &&
         !blocked_throttles_.empty()) {
    UnblockThrottle(blocked_throttles_.front());
  }
}
