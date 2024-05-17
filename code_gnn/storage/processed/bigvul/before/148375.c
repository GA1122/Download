WebContents* WebContents::FromFrameTreeNodeId(int frame_tree_node_id) {
  FrameTreeNode* frame_tree_node =
      FrameTreeNode::GloballyFindByID(frame_tree_node_id);
  if (!frame_tree_node)
    return nullptr;
  return WebContentsImpl::FromFrameTreeNode(frame_tree_node);
}
