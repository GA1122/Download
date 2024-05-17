GetAllNotificationsObserver::GetAllNotificationsObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message) {
  if (AreActiveNotificationProcessesReady()) {
    SendMessage();
  } else {
    registrar_.Add(this, chrome::NOTIFICATION_NOTIFY_BALLOON_CONNECTED,
                   content::NotificationService::AllSources());
  }
}
