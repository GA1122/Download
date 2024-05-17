std::string HeadlessWebContentsImpl::GetMainFrameDevToolsId() const {
  return web_contents()->GetMainFrame()->GetDevToolsFrameToken().ToString();
}
