bool RenderFrameHostManager::InitRenderFrame(
    RenderFrameHostImpl* render_frame_host) {
  if (render_frame_host->IsRenderFrameLive())
    return true;

  SiteInstance* site_instance = render_frame_host->GetSiteInstance();

  int opener_routing_id = MSG_ROUTING_NONE;
  if (frame_tree_node_->opener())
    opener_routing_id = GetOpenerRoutingID(site_instance);

  int parent_routing_id = MSG_ROUTING_NONE;
  if (frame_tree_node_->parent()) {
    parent_routing_id = frame_tree_node_->parent()
                            ->render_manager()
                            ->GetRoutingIdForSiteInstance(site_instance);
    CHECK_NE(parent_routing_id, MSG_ROUTING_NONE);
  }

  int previous_sibling_routing_id = MSG_ROUTING_NONE;
  FrameTreeNode* previous_sibling = frame_tree_node_->PreviousSibling();
  if (previous_sibling) {
    previous_sibling_routing_id =
        previous_sibling->render_manager()->GetRoutingIdForSiteInstance(
            site_instance);
    CHECK_NE(previous_sibling_routing_id, MSG_ROUTING_NONE);
  }

  int proxy_routing_id = MSG_ROUTING_NONE;
  RenderFrameProxyHost* existing_proxy = GetRenderFrameProxyHost(site_instance);
  if (existing_proxy) {
    proxy_routing_id = existing_proxy->GetRoutingID();
    CHECK_NE(proxy_routing_id, MSG_ROUTING_NONE);
    if (!existing_proxy->is_render_frame_proxy_live())
      existing_proxy->InitRenderFrameProxy();
  }

  return delegate_->CreateRenderFrameForRenderManager(
      render_frame_host, proxy_routing_id, opener_routing_id, parent_routing_id,
      previous_sibling_routing_id);
}
