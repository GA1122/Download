RenderFrameHostManager::CreateRenderFrameHost(
    SiteInstance* site_instance,
    int32_t view_routing_id,
    int32_t frame_routing_id,
    int32_t widget_routing_id,
    bool hidden,
    bool renderer_initiated_creation) {
  if (frame_routing_id == MSG_ROUTING_NONE)
    frame_routing_id = site_instance->GetProcess()->GetNextRoutingID();

  FrameTree* frame_tree = frame_tree_node_->frame_tree();
  RenderViewHostImpl* render_view_host = nullptr;
  if (frame_tree_node_->IsMainFrame()) {
    render_view_host = frame_tree->CreateRenderViewHost(
        site_instance, view_routing_id, frame_routing_id, false, hidden);
    if (view_routing_id == MSG_ROUTING_NONE) {
      widget_routing_id = render_view_host->GetRoutingID();
    } else {
      DCHECK_EQ(view_routing_id, render_view_host->GetRoutingID());
    }
  } else {
    render_view_host = frame_tree->GetRenderViewHost(site_instance);
    CHECK(render_view_host);
  }

  return RenderFrameHostFactory::Create(
      site_instance, render_view_host, render_frame_delegate_,
      render_widget_delegate_, frame_tree, frame_tree_node_, frame_routing_id,
      widget_routing_id, hidden, renderer_initiated_creation);
}
