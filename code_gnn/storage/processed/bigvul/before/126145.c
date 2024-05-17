void BrowserLauncherItemController::Launch(int event_flags) {
  DCHECK(!app_id().empty());
  launcher_controller()->LaunchApp(app_id(), event_flags);
}
