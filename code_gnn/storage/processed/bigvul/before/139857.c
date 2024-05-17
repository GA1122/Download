bool Shell::ShouldSaveDisplaySettings() {
  return !(screen_orientation_controller_
               ->ignore_display_configuration_updates() ||
           resolution_notification_controller_->DoesNotificationTimeout());
}
