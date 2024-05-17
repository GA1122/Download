bool ExecuteBrowserCommandObserver::Register(int command) {
  if (!Getint(command, &notification_type_))
    return false;
  registrar_.Add(this, notification_type_,
                 content::NotificationService::AllSources());
  return true;
}
