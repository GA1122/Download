void Browser::UpdateCommandsForBookmarkBar() {
#if !defined(OS_MACOSX)
  const bool show_main_ui = is_type_tabbed() &&
                            (!window_ || !window_->IsFullscreen());
#else
  const bool show_main_ui = is_type_tabbed();
#endif
  command_updater_.UpdateCommandEnabled(IDC_SHOW_BOOKMARK_BAR,
      browser_defaults::bookmarks_enabled &&
      !profile_->GetPrefs()->IsManagedPreference(prefs::kEnableBookmarkBar) &&
      show_main_ui);
}
