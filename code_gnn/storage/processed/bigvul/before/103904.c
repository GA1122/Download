void RenderView::focusedNodeChanged(const WebNode& node) {
  Send(new ViewHostMsg_FocusedNodeChanged(routing_id_, IsEditableNode(node)));

  if (WebAccessibilityCache::accessibilityEnabled() && node.isNull()) {
    postAccessibilityNotification(
        webview()->accessibilityObject(),
        WebKit::WebAccessibilityNotificationFocusedUIElementChanged);
  }

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, FocusedNodeChanged(node));
}
