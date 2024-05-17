void LoginDisplayHostWebUI::StartWizard(OobeScreen first_screen) {
  DisableKeyboardOverscroll();

  TryToPlayOobeStartupSound();

  restore_path_ = RESTORE_WIZARD;
  first_screen_ = first_screen;
  is_showing_login_ = false;

  if (waiting_for_wallpaper_load_ && !initialize_webui_hidden_) {
    VLOG(1) << "Login WebUI >> wizard postponed";
    return;
  }
  VLOG(1) << "Login WebUI >> wizard";

  if (!login_window_)
    LoadURL(GURL(kOobeURL));

  DVLOG(1) << "Starting wizard, first_screen: "
           << GetOobeScreenName(first_screen);
  wizard_controller_.reset();
  wizard_controller_.reset(CreateWizardController());

  oobe_progress_bar_visible_ = !StartupUtils::IsDeviceRegistered();
  SetOobeProgressBarVisible(oobe_progress_bar_visible_);
  wizard_controller_->Init(first_screen);
}
