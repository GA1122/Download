void BrowserCommandController::UpdateCommandsForBookmarkBar() {
  const bool show_main_ui =
      IsShowingMainUI(window() && window()->IsFullscreen());
  command_updater_.UpdateCommandEnabled(IDC_SHOW_BOOKMARK_BAR,
      browser_defaults::bookmarks_enabled &&
      !profile()->GetPrefs()->IsManagedPreference(prefs::kShowBookmarkBar) &&
      show_main_ui);
}
