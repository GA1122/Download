void RenderView::didFailLoad(WebFrame* frame, const WebURLError& error) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidFailLoad(frame, error));
}
