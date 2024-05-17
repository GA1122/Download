void BrowserLauncherItemController::Init() {
  tab_model_->AddObserver(this);
  ash::LauncherItemStatus app_status =
      ash::wm::IsActiveWindow(window_) ?
      ash::STATUS_ACTIVE : ash::STATUS_RUNNING;
  if (type() != TYPE_TABBED) {
    launcher_controller()->CreateAppLauncherItem(this, app_id(), app_status);
  } else {
    launcher_controller()->CreateTabbedLauncherItem(
        this,
        is_incognito_ ? ChromeLauncherController::STATE_INCOGNITO :
                        ChromeLauncherController::STATE_NOT_INCOGNITO,
        app_status);
  }
  if (tab_model_->GetActiveTabContents())
    UpdateLauncher(tab_model_->GetActiveTabContents());
}
