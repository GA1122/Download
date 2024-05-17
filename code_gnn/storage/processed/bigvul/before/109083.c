void RenderViewImpl::didCancelClientRedirect(WebFrame* frame) {
  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidCancelClientRedirect(frame));
}
