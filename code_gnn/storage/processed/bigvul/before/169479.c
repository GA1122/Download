bool NetworkThrottleManagerImpl::ThrottleImpl::IsBlocked() const {
  return state_ == State::BLOCKED;
}
