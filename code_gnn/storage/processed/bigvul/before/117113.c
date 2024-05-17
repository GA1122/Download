bool SessionService::should_track_changes_for_browser_type(Browser::Type type) {
  return type == Browser::TYPE_TABBED ||
        (type == Browser::TYPE_POPUP && browser_defaults::kRestorePopups);
}
