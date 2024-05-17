void RenderViewImpl::OnCopy() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Copy"),
                                            context_menu_node_);
}
