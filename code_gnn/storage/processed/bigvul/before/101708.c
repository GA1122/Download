void Browser::NotifyFullscreenChange() {
  NotificationService::current()->Notify(
      chrome::NOTIFICATION_FULLSCREEN_CHANGED,
      Source<Browser>(this),
      NotificationService::NoDetails());
}
