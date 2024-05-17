bool CommandBufferProxyImpl::SetParent(
    CommandBufferProxy* parent_command_buffer,
    uint32 parent_texture_id) {
  if (last_state_.error != gpu::error::kNoError)
    return false;

  bool result;
  if (parent_command_buffer) {
    if (!Send(new GpuCommandBufferMsg_SetParent(
        route_id_,
        parent_command_buffer->GetRouteID(),
        parent_texture_id,
        &result))) {
      return false;
    }
  } else {
    if (!Send(new GpuCommandBufferMsg_SetParent(
        route_id_,
        MSG_ROUTING_NONE,
        0,
        &result))) {
      return false;
    }
  }

  return result;
}
