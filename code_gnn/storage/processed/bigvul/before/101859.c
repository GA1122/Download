void Browser::UpdateCommandsForBookmarkEditing() {
  bool enabled =
      profile_->GetPrefs()->GetBoolean(prefs::kEditBookmarksEnabled) &&
      browser_defaults::bookmarks_enabled;

  command_updater_.UpdateCommandEnabled(IDC_BOOKMARK_PAGE,
      enabled && is_type_tabbed());
  command_updater_.UpdateCommandEnabled(IDC_BOOKMARK_ALL_TABS,
      enabled && CanBookmarkAllTabs());
}
