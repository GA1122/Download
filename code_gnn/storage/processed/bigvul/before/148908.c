void RenderFrameHostManager::RenderProcessGone(SiteInstanceImpl* instance) {
  GetRenderFrameProxyHost(instance)->set_render_frame_proxy_created(false);
}
