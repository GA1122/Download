void CommandBufferProxyImpl::OnDestroyed(gpu::error::ContextLostReason reason,
                                         gpu::error::Error error) {
  base::AutoLock lock(last_state_lock_);
  OnGpuAsyncMessageError(reason, error);
}
