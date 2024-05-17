WebContentsImpl* WebContentsImpl::FromOuterFrameTreeNode(
    const FrameTreeNode* frame_tree_node) {
  return WebContentsImpl::FromFrameTreeNode(frame_tree_node)
      ->node_.GetInnerWebContentsInFrame(frame_tree_node);
}
