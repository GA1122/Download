void OpenSettingsFromNotification() {
  ShellPort::Get()->RecordUserMetricsAction(
      UMA_STATUS_AREA_DISPLAY_NOTIFICATION_SELECTED);
  if (Shell::Get()->session_controller()->ShouldEnableSettings()) {
    Shell::Get()->system_tray_controller()->ShowDisplaySettings();
    ShellPort::Get()->RecordUserMetricsAction(
        UMA_STATUS_AREA_DISPLAY_NOTIFICATION_SHOW_SETTINGS);
  }
}
