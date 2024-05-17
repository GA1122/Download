OnNotificationBalloonCountObserver::OnNotificationBalloonCountObserver(
    AutomationProvider* provider,
    IPC::Message* reply_message,
    int count)
    : automation_(provider->AsWeakPtr()),
      reply_message_(reply_message),
      collection_(
          g_browser_process->notification_ui_manager()->balloon_collection()),
      count_(count) {
  registrar_.Add(this, chrome::NOTIFICATION_NOTIFY_BALLOON_CONNECTED,
                 content::NotificationService::AllSources());
  collection_->set_on_collection_changed_callback(
      base::Bind(&OnNotificationBalloonCountObserver::CheckBalloonCount,
                 base::Unretained(this)));
  CheckBalloonCount();
}
