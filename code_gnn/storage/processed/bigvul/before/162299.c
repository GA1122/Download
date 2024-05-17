void CommandBufferProxyImpl::TryUpdateStateDontReportError() {
  last_state_lock_.AssertAcquired();
  if (last_state_.error == gpu::error::kNoError)
    shared_state()->Read(&last_state_);
}
