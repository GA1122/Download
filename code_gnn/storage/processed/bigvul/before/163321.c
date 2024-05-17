int RenderThreadImpl::GenerateRoutingID() {
  int32_t routing_id = MSG_ROUTING_NONE;
  render_message_filter()->GenerateRoutingID(&routing_id);
  return routing_id;
}
