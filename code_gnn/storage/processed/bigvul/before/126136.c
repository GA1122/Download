BrowserLauncherItemController::BrowserLauncherItemController(
    Type type,
    aura::Window* window,
    TabStripModel* tab_model,
    ChromeLauncherController* launcher_controller,
    const std::string& app_id)
    : LauncherItemController(type, app_id, launcher_controller),
      window_(window),
      tab_model_(tab_model),
      is_incognito_(tab_model->profile()->GetOriginalProfile() !=
                    tab_model->profile() && !Profile::IsGuestSession()) {
  DCHECK(window_);
  window_->AddObserver(this);
}
