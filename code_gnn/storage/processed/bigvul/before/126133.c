void BrowserLauncherItemController::Activate() {
  window_->Show();
  ash::wm::ActivateWindow(window_);
}
