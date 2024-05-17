void BrowserLauncherItemController::UpdateAppState(content::WebContents* tab) {
  ChromeLauncherController::AppState app_state;

  if (tab_model_->GetIndexOfWebContents(tab) == TabStripModel::kNoTab) {
    app_state = ChromeLauncherController::APP_STATE_REMOVED;
  } else if (tab_model_->GetActiveWebContents() == tab) {
    if (ash::wm::IsActiveWindow(window_))
      app_state = ChromeLauncherController::APP_STATE_WINDOW_ACTIVE;
    else
      app_state = ChromeLauncherController::APP_STATE_ACTIVE;
  } else {
    app_state = ChromeLauncherController::APP_STATE_INACTIVE;
  }
  launcher_controller()->UpdateAppState(tab, app_state);
}
