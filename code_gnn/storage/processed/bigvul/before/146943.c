WebLocalFrame* WebLocalFrame::CreateMainFrame(
    WebView* web_view,
    WebFrameClient* client,
    InterfaceRegistry* interface_registry,
    WebFrame* opener,
    const WebString& name,
    WebSandboxFlags sandbox_flags) {
  return WebLocalFrameImpl::CreateMainFrame(
      web_view, client, interface_registry, opener, name, sandbox_flags);
}
