void RenderFrameDevToolsAgentHost::OnVisibilityChanged(
    content::Visibility visibility) {
#if defined(OS_ANDROID)
  if (visibility == content::Visibility::HIDDEN)
    GetWakeLock()->CancelWakeLock();
  else
    GetWakeLock()->RequestWakeLock();
#endif
}
