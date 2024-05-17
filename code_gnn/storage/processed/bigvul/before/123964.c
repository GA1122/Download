void RenderViewImpl::didBlur() {
  if (webview() && webview()->mainFrame() &&
      webview()->mainFrame()->isProcessingUserGesture() &&
      RenderThreadImpl::current()->should_send_focus_ipcs()) {
    Send(new ViewHostMsg_Blur(routing_id_));
  }
}
