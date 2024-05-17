void RenderViewImpl::didChangeScrollOffset(WebFrame* frame) {
  StartNavStateSyncTimerIfNecessary();

  if (webview()->mainFrame() == frame)
    UpdateScrollState(frame);
}
