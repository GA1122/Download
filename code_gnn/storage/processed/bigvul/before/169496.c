NetworkThrottleManagerImpl::ThrottleImpl::~ThrottleImpl() {
  manager_->OnThrottleDestroyed(this);
}
