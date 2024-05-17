void LocalFrameClientImpl::SetVirtualTimePauser(
    WebScopedVirtualTimePauser virtual_time_pauser) {
  virtual_time_pauser_ = std::move(virtual_time_pauser);
}
