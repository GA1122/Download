bool CanHandleShowMessageCenterBubble() {
  RootWindowController* controller =
      RootWindowController::ForTargetRootWindow();
  StatusAreaWidget* status_area_widget =
      controller->shelf_widget()->status_area_widget();
  return status_area_widget &&
         status_area_widget->web_notification_tray()->visible();
}
