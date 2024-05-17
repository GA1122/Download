void LoginDisplayHostWebUI::OnStartSignInScreen(
    const LoginScreenContext& context) {
  DisableKeyboardOverscroll();

  restore_path_ = RESTORE_SIGN_IN;
  is_showing_login_ = true;
  if (features::IsAshInBrowserProcess())
    finalize_animation_type_ = ANIMATION_WORKSPACE;

  if (waiting_for_wallpaper_load_ && !initialize_webui_hidden_) {
    VLOG(1) << "Login WebUI >> sign in postponed";
    return;
  }
  VLOG(1) << "Login WebUI >> sign in";

  if (!login_window_) {
    TRACE_EVENT_ASYNC_BEGIN0("ui", "ShowLoginWebUI", kShowLoginWebUIid);
    TRACE_EVENT_ASYNC_STEP_INTO0("ui", "ShowLoginWebUI", kShowLoginWebUIid,
                                 "StartSignInScreen");
    BootTimesRecorder::Get()->RecordCurrentStats("login-start-signin-screen");
    LoadURL(GURL(kLoginURL));
  }

  DVLOG(1) << "Starting sign in screen";
  CreateExistingUserController();

  if (!signin_screen_controller_.get()) {
    signin_screen_controller_.reset(new SignInScreenController(GetOobeUI()));
  }

  oobe_progress_bar_visible_ = !StartupUtils::IsDeviceRegistered();
  SetOobeProgressBarVisible(oobe_progress_bar_visible_);
  existing_user_controller_->Init(user_manager::UserManager::Get()->GetUsers());

  CHECK(login_display_);
  GetOobeUI()->ShowSigninScreen(context, login_display_.get(),
                                login_display_.get());
  TRACE_EVENT_ASYNC_STEP_INTO0("ui", "ShowLoginWebUI", kShowLoginWebUIid,
                               "WaitForScreenStateInitialize");

  BootTimesRecorder::Get()->RecordCurrentStats(
      "login-wait-for-signin-state-initialize");
}
