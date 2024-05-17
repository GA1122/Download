bool RenderFrameDevToolsAgentHost::EnsureAgent() {
  if (!frame_host_ || !render_frame_alive_)
    return false;
  if (!agent_ptr_)
    frame_host_->GetRemoteAssociatedInterfaces()->GetInterface(&agent_ptr_);
  return true;
}
