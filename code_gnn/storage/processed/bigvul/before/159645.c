bool RenderFrameHostManager::InitRenderView(
    RenderViewHostImpl* render_view_host,
    RenderFrameProxyHost* proxy) {
  if (!render_view_host->GetProcess()->Init())
    return false;

  if (render_view_host->IsRenderViewLive())
    return true;

  int opener_frame_routing_id =
      GetOpenerRoutingID(render_view_host->GetSiteInstance());

  bool created = delegate_->CreateRenderViewForRenderManager(
      render_view_host, opener_frame_routing_id,
      proxy ? proxy->GetRoutingID() : MSG_ROUTING_NONE,
      frame_tree_node_->devtools_frame_token(),
      frame_tree_node_->current_replication_state());

  if (created && proxy)
    proxy->set_render_frame_proxy_created(true);

  return created;
}
