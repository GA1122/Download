void RenderFrameDevToolsAgentHost::WasHidden() {
#if defined(OS_ANDROID)
  GetWakeLock()->CancelWakeLock();
#endif
}
