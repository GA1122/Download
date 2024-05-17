void RenderFrameHostManager::CreateOuterDelegateProxy(
    SiteInstance* outer_contents_site_instance,
    RenderFrameHostImpl* render_frame_host) {
  RenderFrameProxyHost* proxy =
      CreateRenderFrameProxyHost(outer_contents_site_instance, nullptr);

  render_frame_host->Send(new FrameMsg_SwapOut(
      render_frame_host->GetRoutingID(), proxy->GetRoutingID(),
      false  ,
      render_frame_host->frame_tree_node()->current_replication_state()));
  proxy->set_render_frame_proxy_created(true);

  render_frame_host->SetRenderFrameCreated(false);
}
