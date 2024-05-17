void RenderFrameDevToolsAgentHost::RenderFrameHostChanged(
    RenderFrameHost* old_host,
    RenderFrameHost* new_host) {
  if (old_host != frame_host_)
    return;

  UpdateFrameHost(nullptr);
}
