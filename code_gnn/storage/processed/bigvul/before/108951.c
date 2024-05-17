void RenderViewImpl::OnPaste() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Paste"));
}
