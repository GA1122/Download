bool RenderView::RendererAccessibilityNotification::ShouldIncludeChildren() {
  typedef ViewHostMsg_AccessibilityNotification_Params params;
  if (type == WebKit::WebAccessibilityNotificationChildrenChanged ||
      type == WebKit::WebAccessibilityNotificationLoadComplete) {
    return true;
  }
  return false;
}
