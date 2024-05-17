void RenderFrameDevToolsAgentHost::DetachSession(DevToolsSession* session) {
  if (sessions().empty()) {
    frame_trace_recorder_.reset();
    RevokePolicy();
#if defined(OS_ANDROID)
    GetWakeLock()->CancelWakeLock();
#endif
  }
}
