void RenderViewImpl::DidHandleMouseEvent(const WebMouseEvent& event) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidHandleMouseEvent(event));
}
