void CommandBufferProxyImpl::SignalSyncToken(const gpu::SyncToken& sync_token,
                                             const base::Closure& callback) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  uint32_t signal_id = next_signal_id_++;
  Send(new GpuCommandBufferMsg_SignalSyncToken(route_id_, sync_token,
                                               signal_id));
  signal_tasks_.insert(std::make_pair(signal_id, callback));
}
