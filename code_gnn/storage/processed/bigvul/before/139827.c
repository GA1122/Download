WebNotificationTray* Shell::GetWebNotificationTray() {
  return GetPrimaryRootWindowController()->shelf()->
      status_area_widget()->web_notification_tray();
}
