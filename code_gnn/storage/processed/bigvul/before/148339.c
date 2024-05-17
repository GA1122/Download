void WebContentsImpl::WebContentsTreeNode::DetachInnerWebContents(
    WebContentsImpl* inner_web_contents) {
  DCHECK(std::find(inner_web_contents_.begin(), inner_web_contents_.end(),
                   inner_web_contents) != inner_web_contents_.end());
  inner_web_contents_.erase(
      std::remove(inner_web_contents_.begin(), inner_web_contents_.end(),
                  inner_web_contents),
      inner_web_contents_.end());
}
