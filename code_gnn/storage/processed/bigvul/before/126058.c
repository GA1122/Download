TabStripNotificationObserver::TabStripNotificationObserver(
    int notification, AutomationProvider* automation)
    : automation_(automation->AsWeakPtr()),
      notification_(notification) {
  registrar_.Add(this, notification_,
                 content::NotificationService::AllSources());
}
