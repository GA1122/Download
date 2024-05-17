void CommandBufferProxyImpl::SignalQuery(uint32_t query,
                                         const base::Closure& callback) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  uint32_t signal_id = next_signal_id_++;
  Send(new GpuCommandBufferMsg_SignalQuery(route_id_, query, signal_id));
  signal_tasks_.insert(std::make_pair(signal_id, callback));
}
