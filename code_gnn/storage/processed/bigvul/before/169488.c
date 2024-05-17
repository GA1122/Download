NetworkThrottleManagerImpl::ThrottleImpl::ThrottleImpl(
    bool blocked,
    RequestPriority priority,
    NetworkThrottleManager::ThrottleDelegate* delegate,
    NetworkThrottleManagerImpl* manager,
    ThrottleListQueuePointer queue_pointer)
    : state_(blocked ? State::BLOCKED : State::OUTSTANDING),
      priority_(priority),
      delegate_(delegate),
      manager_(manager),
      queue_pointer_(queue_pointer) {
  DCHECK(delegate);
  if (!blocked)
    start_time_ = manager->tick_clock_->NowTicks();
}
