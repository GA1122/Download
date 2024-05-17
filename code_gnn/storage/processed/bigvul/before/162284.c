void CommandBufferProxyImpl::ReturnFrontBuffer(const gpu::Mailbox& mailbox,
                                               const gpu::SyncToken& sync_token,
                                               bool is_lost) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  Send(new GpuCommandBufferMsg_WaitSyncToken(route_id_, sync_token));
  Send(new GpuCommandBufferMsg_ReturnFrontBuffer(route_id_, mailbox, is_lost));
}
