NewNotificationBalloonObserver::NewNotificationBalloonObserver(
    AutomationProvider* provider,
    IPC::Message* reply_message)
    : automation_(provider->AsWeakPtr()),
      reply_message_(reply_message) {
  registrar_.Add(this, chrome::NOTIFICATION_NOTIFY_BALLOON_CONNECTED,
                 content::NotificationService::AllSources());
}
