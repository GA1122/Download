void HandleShowSystemTrayBubble() {
  base::RecordAction(UserMetricsAction("Accel_Show_System_Tray_Bubble"));
  RootWindowController* controller =
      RootWindowController::ForTargetRootWindow();
  if (!controller->GetSystemTray()->HasSystemBubble())
    controller->GetSystemTray()->ShowDefaultView(BUBBLE_CREATE_NEW);
}
