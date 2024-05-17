void GetAllNotificationsObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }
  if (AreActiveNotificationProcessesReady())
    SendMessage();
}
