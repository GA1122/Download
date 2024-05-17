bool CommandBufferProxyImpl::Send(IPC::Message* msg) {
  DCHECK(channel_);
  last_state_lock_.AssertAcquired();
  DCHECK_EQ(gpu::error::kNoError, last_state_.error);

  last_state_lock_.Release();

  bool is_sync = msg->is_sync();
  bool result = channel_->Send(msg);
  DCHECK(is_sync || result);

  last_state_lock_.Acquire();

  if (last_state_.error != gpu::error::kNoError) {
    if (gpu_control_client_)
      gpu_control_client_->OnGpuControlLostContextMaybeReentrant();
    return false;
  }

  if (!result) {
    DVLOG(1) << "CommandBufferProxyImpl::Send failed. Losing context.";
    OnClientError(gpu::error::kLostContext);
    return false;
  }

  return true;
}
