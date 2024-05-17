TabCloseableStateWatcher::TabCloseableStateWatcher()
    : can_close_tab_(true),
      signing_off_(false),
      guest_session_(
          CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kGuestSession)),
      waiting_for_browser_(false) {
  BrowserList::AddObserver(this);
  notification_registrar_.Add(this, NotificationType::APP_EXITING,
      NotificationService::AllSources());
}
