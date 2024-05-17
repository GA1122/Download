void RenderFrameImpl::FocusedNodeChanged(const WebNode& node) {
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, FocusedNodeChanged(node));
}
