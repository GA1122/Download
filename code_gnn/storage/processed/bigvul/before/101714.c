void Browser::OnStateChanged() {
  DCHECK(profile_->GetProfileSyncService());

#if !defined(OS_MACOSX)
  const bool show_main_ui = is_type_tabbed() && !window_->IsFullscreen();
#else
  const bool show_main_ui = is_type_tabbed();
#endif

  command_updater_.UpdateCommandEnabled(IDC_SYNC_BOOKMARKS,
      show_main_ui && profile_->GetOriginalProfile()->IsSyncAccessible());
}
