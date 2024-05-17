void RenderFrameHostManager::DiscardUnusedFrame(
    std::unique_ptr<RenderFrameHostImpl> render_frame_host) {

  SiteInstanceImpl* site_instance = render_frame_host->GetSiteInstance();
  RenderViewHostImpl* rvh = render_frame_host->render_view_host();
  RenderFrameProxyHost* proxy = nullptr;
  if (site_instance->HasSite() && site_instance->active_frame_count() > 1) {
    proxy = GetRenderFrameProxyHost(site_instance);
    if (!proxy)
      proxy = CreateRenderFrameProxyHost(site_instance, rvh);
  }

  if (frame_tree_node_->IsMainFrame()) {
    rvh->set_main_frame_routing_id(MSG_ROUTING_NONE);
    rvh->set_is_active(false);
    rvh->set_is_swapped_out(true);
  }

  render_frame_host.reset();

  if (proxy && !proxy->is_render_frame_proxy_live())
    proxy->InitRenderFrameProxy();
}
