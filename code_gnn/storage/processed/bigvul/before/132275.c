void RenderFrameImpl::WasHidden() {
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, WasHidden());
}
