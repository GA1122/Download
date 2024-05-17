Browser::~Browser() {
  if (profile_->GetProfileSyncService())
    profile_->GetProfileSyncService()->RemoveObserver(this);

  BrowserList::RemoveBrowser(this);

#if !defined(OS_MACOSX)
  if (!BrowserList::HasBrowserWithProfile(profile_)) {
    TabRestoreServiceFactory::ResetForProfile(profile_);
  }
#endif

  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service)
    session_service->WindowClosed(session_id_);

  TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfile(profile());
  if (tab_restore_service)
    tab_restore_service->BrowserClosed(tab_restore_service_delegate());

  profile_pref_registrar_.RemoveAll();
  local_pref_registrar_.RemoveAll();

  encoding_auto_detect_.Destroy();
  use_vertical_tabs_.Destroy();
  use_compact_navigation_bar_.Destroy();

  if (profile_->IsOffTheRecord() &&
      !BrowserList::IsOffTheRecordSessionActiveForProfile(profile_)) {
    profile_->GetOriginalProfile()->DestroyOffTheRecordProfile();
  }

  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();

  TabRestoreServiceDestroyed(tab_restore_service_);
}
