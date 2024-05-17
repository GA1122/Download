void CommandBufferProxyImpl::TakeFrontBuffer(const gpu::Mailbox& mailbox) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  Send(new GpuCommandBufferMsg_TakeFrontBuffer(route_id_, mailbox));
}
