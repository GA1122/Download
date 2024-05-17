int HeadlessWebContentsImpl::GetMainFrameRenderProcessId() const {
  return web_contents()->GetMainFrame()->GetProcess()->GetID();
}
