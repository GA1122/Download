gpu::CommandBuffer::State CommandBufferProxyImpl::GetState() {
  if (last_state_.error == gpu::error::kNoError) {
    gpu::CommandBuffer::State state;
    if (Send(new GpuCommandBufferMsg_GetState(route_id_, &state)))
      OnUpdateState(state);
  }

  TryUpdateState();
  return last_state_;
}
