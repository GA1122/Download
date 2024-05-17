void WebContentsImpl::WebContentsTreeNode::AttachInnerWebContents(
    WebContentsImpl* inner_web_contents) {
  inner_web_contents_.push_back(inner_web_contents);
}
