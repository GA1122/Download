void RenderView::frameDetached(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, FrameDetached(frame));
}