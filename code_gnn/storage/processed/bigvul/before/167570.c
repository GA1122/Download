void RenderFrameHostManager::Init(SiteInstance* site_instance,
                                  int32_t view_routing_id,
                                  int32_t frame_routing_id,
                                  int32_t widget_routing_id,
                                  bool renderer_initiated_creation) {
  DCHECK(site_instance);
  SetRenderFrameHost(CreateRenderFrameHost(site_instance, view_routing_id,
                                           frame_routing_id, widget_routing_id,
                                           delegate_->IsHidden(),
                                           renderer_initiated_creation));

  if (!frame_tree_node_->IsMainFrame()) {
    delegate_->NotifySwappedFromRenderManager(
        nullptr, render_frame_host_.get(), false);
  }
}
