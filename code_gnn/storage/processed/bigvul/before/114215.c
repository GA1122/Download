bool CommandBufferProxyImpl::DiscardBackbuffer() {
  if (last_state_.error != gpu::error::kNoError)
    return false;

  return Send(new GpuCommandBufferMsg_DiscardBackbuffer(route_id_));
}
