void RenderViewImpl::frameDetached(WebFrame* frame) {
  if (is_loading_) {
    pending_frame_tree_update_ = true;
    FOR_EACH_OBSERVER(RenderViewObserver, observers_, FrameDetached(frame));
    return;
  }
  if (!updating_frame_tree_)
    SendUpdatedFrameTree(frame);

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, FrameDetached(frame));
}
