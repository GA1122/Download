void RenderThreadImpl::OnCreateNewFrameProxy(int routing_id,
                                             int parent_routing_id,
                                             int render_view_routing_id) {
  RenderFrameProxy::CreateFrameProxy(
      routing_id, parent_routing_id, render_view_routing_id);
}
