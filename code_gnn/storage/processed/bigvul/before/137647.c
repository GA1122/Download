void RenderThreadImpl::OnCreateNewFrame(int routing_id, int parent_routing_id) {
  RenderFrameImpl::CreateFrame(routing_id, parent_routing_id);
}
