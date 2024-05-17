BrowserLauncherItemController::~BrowserLauncherItemController() {
  tab_model_->RemoveObserver(this);
  window_->RemoveObserver(this);
  if (launcher_id() > 0)
    launcher_controller()->CloseLauncherItem(launcher_id());
}
