void CommandBufferProxyImpl::OnClientError(gpu::error::Error error) {
  CheckLock();
  last_state_lock_.AssertAcquired();
  last_state_.error = error;
  last_state_.context_lost_reason = gpu::error::kUnknown;
  DisconnectChannelInFreshCallStack();
}
