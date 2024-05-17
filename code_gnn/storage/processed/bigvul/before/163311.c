void RenderThreadImpl::CreateFrameProxy(
    int32_t routing_id,
    int32_t render_view_routing_id,
    int32_t opener_routing_id,
    int32_t parent_routing_id,
    const FrameReplicationState& replicated_state) {
  RenderFrameProxy::CreateFrameProxy(
      routing_id, render_view_routing_id,
      RenderFrameImpl::ResolveOpener(opener_routing_id), parent_routing_id,
      replicated_state);
}
