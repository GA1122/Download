void BrowserCommandController::UpdateCommandsForMultipleProfiles() {
  bool show_main_ui = IsShowingMainUI(window() && window()->IsFullscreen());
  command_updater_.UpdateCommandEnabled(IDC_SHOW_AVATAR_MENU,
      show_main_ui &&
      !profile()->IsOffTheRecord() &&
      ProfileManager::IsMultipleProfilesEnabled());
}
