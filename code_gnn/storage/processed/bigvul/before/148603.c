WebContentsImpl::WebContentsTreeNode::~WebContentsTreeNode() {
  if (OuterContentsFrameTreeNode())
    OuterContentsFrameTreeNode()->RemoveObserver(this);

  if (outer_web_contents_)
    outer_web_contents_->node_.DetachInnerWebContents(current_web_contents_);
}
