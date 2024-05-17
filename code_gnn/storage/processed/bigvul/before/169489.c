void NetworkThrottleManagerImpl::UnblockThrottle(ThrottleImpl* throttle) {
  DCHECK(throttle->IsBlocked());

  blocked_throttles_.erase(throttle->queue_pointer());
  throttle->set_start_time(tick_clock_->NowTicks());
  throttle->set_queue_pointer(
      outstanding_throttles_.insert(outstanding_throttles_.end(), throttle));

  RecomputeOutstanding();

  throttle->NotifyUnblocked();
}
