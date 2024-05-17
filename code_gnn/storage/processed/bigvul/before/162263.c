CommandBuffer::State CommandBufferProxyImpl::GetLastState() {
  base::AutoLock lock(last_state_lock_);
  TryUpdateState();
  return last_state_;
}
