gpu::CommandBuffer::State CommandBufferProxyImpl::FlushSync(
    int32 put_offset,
    int32 last_known_get) {
  TRACE_EVENT1("gpu", "CommandBufferProxyImpl::FlushSync", "put_offset",
               put_offset);
  Flush(put_offset);
  TryUpdateState();
  if (last_known_get == last_state_.get_offset) {
    if (last_state_.error == gpu::error::kNoError) {
      gpu::CommandBuffer::State state;
      if (Send(new GpuCommandBufferMsg_GetStateFast(route_id_,
                                                    &state)))
        OnUpdateState(state);
    }
    TryUpdateState();
  }

  return last_state_;
}
