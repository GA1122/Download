void CommandBufferProxyImpl::TryUpdateState() {
  if (last_state_.error == gpu::error::kNoError)
    shared_state_->Read(&last_state_);
}
