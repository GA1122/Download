void BrowserActionButton::Observe(NotificationType type,
                                  const NotificationSource& source,
                                  const NotificationDetails& details) {
  if (type == NotificationType::EXTENSION_BROWSER_ACTION_UPDATED) {
    UpdateState();
    panel_->OnBrowserActionVisibilityChanged();
  } else {
    NOTREACHED() << L"Received unexpected notification";
  }
}
