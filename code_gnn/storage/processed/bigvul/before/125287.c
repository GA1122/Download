void RenderViewImpl::didChangeScrollOffset(WebFrame* frame) {
  StartNavStateSyncTimerIfNecessary();

  if (webview()->mainFrame() == frame)
    UpdateScrollState(frame);

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidChangeScrollOffset(frame));

#if defined(OS_ANDROID)
  if (webview()->mainFrame() == frame)
    ScheduleUpdateFrameInfo();
#endif
}
