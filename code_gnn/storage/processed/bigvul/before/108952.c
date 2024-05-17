void RenderViewImpl::OnPasteAndMatchStyle() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(
      WebString::fromUTF8("PasteAndMatchStyle"));
}
