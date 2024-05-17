void Browser::OpenSyncMyBookmarksDialog() {
  sync_ui_util::OpenSyncMyBookmarksDialog(
      profile_, this, ProfileSyncService::START_FROM_WRENCH);
}
