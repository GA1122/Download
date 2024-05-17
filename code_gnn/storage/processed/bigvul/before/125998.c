void OnNotificationBalloonCountObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  CheckBalloonCount();
}
