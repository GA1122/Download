void SessionService::Save() {
  bool had_commands = !pending_commands().empty();
  BaseSessionService::Save();
  if (had_commands) {
    RecordSessionUpdateHistogramData(
        chrome::NOTIFICATION_SESSION_SERVICE_SAVED,
        &last_updated_save_time_);
    content::NotificationService::current()->Notify(
        chrome::NOTIFICATION_SESSION_SERVICE_SAVED,
        content::Source<Profile>(profile()),
        content::NotificationService::NoDetails());
  }
}
