void TabCloseableStateWatcher::Observe(NotificationType type,
    const NotificationSource& source, const NotificationDetails& details) {
  if (type.value != NotificationType::APP_EXITING)
    NOTREACHED();
  if (!signing_off_) {
    signing_off_ = true;
    SetCloseableState(true);
  }
}
