void WindowedLoadStopObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (--remaining_notification_count_ == 0)
    WindowedNotificationObserver::Observe(type, source, details);
}
