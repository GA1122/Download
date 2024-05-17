int HeadlessWebContentsImpl::GetMainFrameTreeNodeId() const {
  return web_contents()->GetMainFrame()->GetFrameTreeNodeId();
}
