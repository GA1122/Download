void BrowserCommandController::UpdateCommandsForLockedFullscreenMode() {
  bool is_locked_fullscreen = ash::IsWindowTrustedPinned(browser_->window());
  DCHECK_NE(is_locked_fullscreen, is_locked_fullscreen_);
  if (is_locked_fullscreen == is_locked_fullscreen_)
    return;
  is_locked_fullscreen_ = is_locked_fullscreen;

  if (is_locked_fullscreen_) {
    command_updater_.DisableAllCommands();
    UpdateCommandsForContentRestrictionState();
    UpdateCommandsForFind();
    UpdateCommandsForZoomState();
#if DCHECK_IS_ON()
    NonWhitelistedCommandsAreDisabled(&command_updater_);
#endif
  } else {
    InitCommandState();
  }
}
