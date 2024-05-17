bool RenderFrameDevToolsAgentHost::IsFrameHostAllowedForRestrictedSessions() {
  return !frame_host_ ||
         (!frame_host_->web_ui() && !frame_host_->pending_web_ui());
}
