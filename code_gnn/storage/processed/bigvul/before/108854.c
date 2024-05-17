void RenderViewImpl::DidHandleTouchEvent(const WebTouchEvent& event) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidHandleTouchEvent(event));
}
