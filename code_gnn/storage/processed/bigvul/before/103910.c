void RenderView::printPage(WebFrame* frame) {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, PrintPage(frame));
}
