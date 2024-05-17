void WebContentsImpl::AccessibilityLocationChangesReceived(
    const std::vector<AXLocationChangeNotificationDetails>& details) {
  for (auto& observer : observers_)
    observer.AccessibilityLocationChangesReceived(details);
}
