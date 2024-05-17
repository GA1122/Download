void RenderViewImpl::didCreateFrame(WebFrame* parent, WebFrame* child) {
  if (!updating_frame_tree_)
    SendUpdatedFrameTree(NULL);
}
