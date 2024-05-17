void RenderFrameDevToolsAgentHost::RenderFrameDeleted(RenderFrameHost* rfh) {
  if (rfh == frame_host_) {
    render_frame_alive_ = false;
    UpdateRendererChannel(IsAttached());
  }
}
