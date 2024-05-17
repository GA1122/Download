void WebContentsImpl::AccessibilityEventReceived(
    const AXEventNotificationDetails& details) {
  for (auto& observer : observers_)
    observer.AccessibilityEventReceived(details);
}
