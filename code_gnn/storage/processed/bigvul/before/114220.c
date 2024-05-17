gpu::error::Error CommandBufferProxyImpl::GetLastError() {
  return last_state_.error;
}
