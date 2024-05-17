void HandleShowMessageCenterBubble() {
  base::RecordAction(UserMetricsAction("Accel_Show_Message_Center_Bubble"));
  RootWindowController* controller =
      RootWindowController::ForTargetRootWindow();
  StatusAreaWidget* status_area_widget =
      controller->shelf_widget()->status_area_widget();
  if (status_area_widget) {
    WebNotificationTray* notification_tray =
        status_area_widget->web_notification_tray();
    if (notification_tray->visible())
      notification_tray->ShowMessageCenterBubble();
  }
}
