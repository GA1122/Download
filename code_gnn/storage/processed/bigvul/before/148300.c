void WebContentsImpl::AccessibilityEventReceived(
    const std::vector<AXEventNotificationDetails>& details) {
  for (auto& observer : observers_)
    observer.AccessibilityEventReceived(details);
}
