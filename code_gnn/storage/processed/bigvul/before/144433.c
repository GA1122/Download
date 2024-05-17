void WebContentsImpl::AccessibilityEventReceived(
    const std::vector<AXEventNotificationDetails>& details) {
  FOR_EACH_OBSERVER(
      WebContentsObserver, observers_, AccessibilityEventReceived(details));
}
