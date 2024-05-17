void Browser::UpdateCommandsForBookmarkEditing() {
  bool enabled = edit_bookmarks_enabled_.GetValue() &&
                 browser_defaults::bookmarks_enabled;

  command_updater_.UpdateCommandEnabled(IDC_BOOKMARK_PAGE,
      enabled && type() == TYPE_NORMAL);
  command_updater_.UpdateCommandEnabled(IDC_BOOKMARK_ALL_TABS,
      enabled && CanBookmarkAllTabs());
}
