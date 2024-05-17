NetworkThrottleManagerImpl::CreateThrottle(
    NetworkThrottleManager::ThrottleDelegate* delegate,
    RequestPriority priority,
    bool ignore_limits) {
  bool blocked =
      (!ignore_limits && priority == THROTTLED &&
       outstanding_throttles_.size() >= kActiveRequestThrottlingLimit);

  std::unique_ptr<NetworkThrottleManagerImpl::ThrottleImpl> throttle(
      new ThrottleImpl(blocked, priority, delegate, this,
                       blocked_throttles_.end()));

  ThrottleList& insert_list(blocked ? blocked_throttles_
                                    : outstanding_throttles_);

  throttle->set_queue_pointer(
      insert_list.insert(insert_list.end(), throttle.get()));

  if (!blocked)
    RecomputeOutstanding();

  return std::move(throttle);
}
