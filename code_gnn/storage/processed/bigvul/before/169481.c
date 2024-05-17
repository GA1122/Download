void NetworkThrottleManagerImpl::ThrottleImpl::NotifyUnblocked() {
  DCHECK_EQ(State::BLOCKED, state_);
  state_ = State::OUTSTANDING;
  delegate_->OnThrottleUnblocked(this);
}
