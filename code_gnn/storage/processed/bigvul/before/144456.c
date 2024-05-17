bool WebContentsImpl::CreateRenderFrameForRenderManager(
    RenderFrameHost* render_frame_host,
    int proxy_routing_id,
    int opener_routing_id,
    int parent_routing_id,
    int previous_sibling_routing_id) {
  TRACE_EVENT0("browser,navigation",
               "WebContentsImpl::CreateRenderFrameForRenderManager");

  RenderFrameHostImpl* rfh =
      static_cast<RenderFrameHostImpl*>(render_frame_host);
  if (!rfh->CreateRenderFrame(proxy_routing_id, opener_routing_id,
                              parent_routing_id, previous_sibling_routing_id))
    return false;


  return true;
}
