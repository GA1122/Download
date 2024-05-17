void WebContentsImpl::WebContentsTreeNode::OnFrameTreeNodeDestroyed(
    FrameTreeNode* node) {
  DCHECK_EQ(outer_contents_frame_tree_node_id_, node->frame_tree_node_id())
      << "WebContentsTreeNode should only receive notifications for the "
         "FrameTreeNode in its outer WebContents that hosts it.";
  delete current_web_contents_;   
}
