void RenderViewImpl::OnUnselect() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Unselect"));
}
