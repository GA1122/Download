void Browser::OnStateChanged() {
  DCHECK(profile_->GetProfileSyncService());

#if !defined(OS_MACOSX)
  const bool show_main_ui = (type() == TYPE_NORMAL) && !window_->IsFullscreen();
#else
  const bool show_main_ui = (type() == TYPE_NORMAL);
#endif

  command_updater_.UpdateCommandEnabled(IDC_SYNC_BOOKMARKS,
      show_main_ui && profile_->IsSyncAccessible());
}
