void CommandBufferProxyImpl::OnChannelError() {
  base::Optional<base::AutoLock> lock;
  if (lock_)
    lock.emplace(*lock_);
  base::AutoLock last_state_lock(last_state_lock_);

  gpu::error::ContextLostReason context_lost_reason =
      gpu::error::kGpuChannelLost;
  if (shared_state_shm_ && shared_state_shm_->memory()) {
    TryUpdateStateDontReportError();
    if (last_state_.error == gpu::error::kLostContext)
      context_lost_reason = last_state_.context_lost_reason;
  }
  OnGpuAsyncMessageError(context_lost_reason, gpu::error::kLostContext);
}
