void RenderFrameDevToolsAgentHost::RenderFrameHostChanged(
    RenderFrameHost* old_host,
    RenderFrameHost* new_host) {
  for (auto* target : protocol::TargetHandler::ForAgentHost(this))
    target->RenderFrameHostChanged();

  if (old_host != frame_host_)
    return;

  UpdateFrameHost(nullptr);
}
