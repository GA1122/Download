void LoginDisplayHostWebUI::OnStartArcKiosk() {
  if (features::IsAshInBrowserProcess())
    finalize_animation_type_ = ANIMATION_FADE_OUT;
  if (!login_window_) {
    LoadURL(GURL(kAppLaunchSplashURL));
    LoadURL(GURL(kArcKioskSplashURL));
  }

  login_view_->set_should_emit_login_prompt_visible(false);
}
