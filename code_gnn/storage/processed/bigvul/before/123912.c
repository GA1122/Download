void RenderViewImpl::OnSetName(const std::string& name) {
  if (!webview())
    return;

  webview()->mainFrame()->setName(WebString::fromUTF8(name));
}
