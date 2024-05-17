void Browser::ToggleFullscreenMode() {
#if !defined(OS_MACOSX)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode) &&
      window_->IsFullscreen())
    return;
#endif

  UserMetrics::RecordAction(UserMetricsAction("ToggleFullscreen"), profile_);
  window_->SetFullscreen(!window_->IsFullscreen());
#if !defined(OS_LINUX)
  UpdateCommandsForFullscreenMode(window_->IsFullscreen());
#endif
}
