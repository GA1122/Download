void RenderViewImpl::didFocus() {
  if (webview() && webview()->mainFrame() &&
      webview()->mainFrame()->isProcessingUserGesture()) {
    Send(new ViewHostMsg_Focus(routing_id_));
  }
}
