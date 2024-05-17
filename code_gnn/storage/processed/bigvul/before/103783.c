void RenderView::DidHandleMouseEvent(const WebKit::WebMouseEvent& event) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidHandleMouseEvent(event));
}
