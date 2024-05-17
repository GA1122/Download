void NetworkThrottleManagerImpl::ThrottleImpl::SetAged() {
  DCHECK_EQ(State::OUTSTANDING, state_);
  state_ = State::AGED;
}
