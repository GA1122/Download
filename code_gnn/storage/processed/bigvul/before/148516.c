WebContentsImpl::WebContentsTreeNode::OuterContentsFrameTreeNode() const {
  return FrameTreeNode::GloballyFindByID(outer_contents_frame_tree_node_id_);
}
