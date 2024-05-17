WebContentsImpl::WebContentsTreeNode::GetInnerWebContentsInFrame(
    const FrameTreeNode* frame) {
  auto ftn_id = frame->frame_tree_node_id();
  for (WebContentsImpl* contents : inner_web_contents_) {
    if (contents->node_.outer_contents_frame_tree_node_id() == ftn_id) {
      return contents;
    }
  }
  return nullptr;
}
