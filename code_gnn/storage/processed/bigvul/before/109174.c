void RenderViewImpl::willClose(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, FrameWillClose(frame));
}
