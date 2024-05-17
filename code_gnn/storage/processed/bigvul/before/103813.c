void RenderView::OnEnableAccessibility() {
  if (WebAccessibilityCache::accessibilityEnabled())
    return;

  WebAccessibilityCache::enableAccessibility();

  if (webview()) {
    postAccessibilityNotification(
        webview()->accessibilityObject(),
        WebKit::WebAccessibilityNotificationLoadComplete);
  }
}
