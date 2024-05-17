void Browser::OpenSyncMyBookmarksDialog() {
  sync_ui_util::OpenSyncMyBookmarksDialog(
      profile_, ProfileSyncService::START_FROM_WRENCH);
}
