bool CommandBufferProxyImpl::SetSurfaceVisible(bool visible) {
  if (last_state_.error != gpu::error::kNoError)
    return false;

  return Send(new GpuCommandBufferMsg_SetSurfaceVisible(route_id_, visible));
}
