void RenderViewImpl::OnStopFinding(StopFindAction action) {
#if defined(OS_ANDROID)
  if (synchronous_find_reply_message_.get())
    return;
#endif

  StopFinding(action);
}
