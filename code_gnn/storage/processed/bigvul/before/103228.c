Browser::~Browser() {
  VLOG_IF(1, g_log_bug53991) << "~Browser: " << profile_->IsOffTheRecord()
                             << "; stillActive="
                             << BrowserList::IsOffTheRecordSessionActive();

  if (profile_->GetProfileSyncService())
    profile_->GetProfileSyncService()->RemoveObserver(this);

  BrowserList::RemoveBrowser(this);

#if defined(OS_WIN) || defined(OS_LINUX)
  if (!BrowserList::HasBrowserWithProfile(profile_)) {
    profile_->ResetTabRestoreService();
  }
#endif

  SessionService* session_service = profile_->GetSessionService();
  if (session_service)
    session_service->WindowClosed(session_id_);

  TabRestoreService* tab_restore_service = profile()->GetTabRestoreService();
  if (tab_restore_service)
    tab_restore_service->BrowserClosed(tab_restore_service_delegate());

  encoding_auto_detect_.Destroy();
  printing_enabled_.Destroy();
  dev_tools_disabled_.Destroy();
  incognito_mode_allowed_.Destroy();
  instant_enabled_.Destroy();
  use_vertical_tabs_.Destroy();
  edit_bookmarks_enabled_.Destroy();

  if (profile_->IsOffTheRecord() &&
      !BrowserList::IsOffTheRecordSessionActive()) {
    profile_->GetOriginalProfile()->DestroyOffTheRecordProfile();
  }

  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();

  TabRestoreServiceDestroyed(tab_restore_service_);
}
