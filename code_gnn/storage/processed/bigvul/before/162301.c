gpu::CommandBuffer::State CommandBufferProxyImpl::WaitForGetOffsetInRange(
    uint32_t set_get_buffer_count,
    int32_t start,
    int32_t end) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  TRACE_EVENT2("gpu", "CommandBufferProxyImpl::WaitForGetOffset", "start",
               start, "end", end);
  if (last_state_.error != gpu::error::kNoError) {
    if (gpu_control_client_)
      gpu_control_client_->OnGpuControlLostContextMaybeReentrant();
    return last_state_;
  }
  TryUpdateState();
  if (((set_get_buffer_count != last_state_.set_get_buffer_count) ||
       !InRange(start, end, last_state_.get_offset)) &&
      last_state_.error == gpu::error::kNoError) {
    gpu::CommandBuffer::State state;
    if (Send(new GpuCommandBufferMsg_WaitForGetOffsetInRange(
            route_id_, set_get_buffer_count, start, end, &state)))
      SetStateFromMessageReply(state);
  }
  if (((set_get_buffer_count != last_state_.set_get_buffer_count) ||
       !InRange(start, end, last_state_.get_offset)) &&
      last_state_.error == gpu::error::kNoError) {
    LOG(ERROR) << "GPU state invalid after WaitForGetOffsetInRange.";
    OnGpuSyncReplyError();
  }
  return last_state_;
}
