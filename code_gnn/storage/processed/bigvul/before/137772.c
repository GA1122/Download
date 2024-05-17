void HeadlessWebContentsImpl::RenderFrameDeleted(
    content::RenderFrameHost* render_frame_host) {
  if (headless_tab_socket_)
    headless_tab_socket_->RenderFrameDeleted(render_frame_host);
  browser_context_->RemoveDevToolsFrameToken(
      render_frame_host->GetProcess()->GetID(),
      render_frame_host->GetRoutingID(),
      render_frame_host->GetFrameTreeNodeId());
}
