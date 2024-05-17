void WebContentsImpl::WebContentsTreeNode::ConnectToOuterWebContents(
    WebContentsImpl* outer_web_contents,
    RenderFrameHostImpl* outer_contents_frame) {
  outer_web_contents_ = outer_web_contents;
  outer_contents_frame_tree_node_id_ =
      outer_contents_frame->frame_tree_node()->frame_tree_node_id();

  if (!outer_web_contents_->node_)
    outer_web_contents_->node_.reset(new WebContentsTreeNode());

  outer_web_contents_->node_->inner_web_contents_tree_nodes_.insert(this);
}
