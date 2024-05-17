void RenderViewImpl::postAccessibilityNotification(
    const WebAccessibilityObject& obj,
    WebAccessibilityNotification notification) {
  if (renderer_accessibility_) {
    renderer_accessibility_->HandleWebAccessibilityNotification(
        obj, notification);
  }
}
