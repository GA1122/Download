void CommandBufferProxyImpl::OnGpuStateError() {
  CheckLock();
  last_state_lock_.AssertAcquired();
  DCHECK_NE(gpu::error::kNoError, last_state_.error);
  DisconnectChannelInFreshCallStack();
}
