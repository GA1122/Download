void BrowserCommandController::UpdateCommandsForBookmarkBar() {
  if (is_locked_fullscreen_)
    return;

  command_updater_.UpdateCommandEnabled(
      IDC_SHOW_BOOKMARK_BAR,
      browser_defaults::bookmarks_enabled && !profile()->IsGuestSession() &&
          !profile()->IsSystemProfile() &&
          !profile()->GetPrefs()->IsManagedPreference(
              bookmarks::prefs::kShowBookmarkBar) &&
          IsShowingMainUI());
}
