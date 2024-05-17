Browser::~Browser() {
  if (!browser_shutdown::ShuttingDownWithoutClosingBrowsers())
    DCHECK(tab_strip_model_->empty());

  search_model_->RemoveObserver(this);
  tab_strip_model_->RemoveObserver(this);

  BrowserList::RemoveBrowser(this);

  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service)
    session_service->WindowClosed(session_id_);

  TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfile(profile());
  if (tab_restore_service)
    tab_restore_service->BrowserClosed(tab_restore_service_delegate());

#if !defined(OS_MACOSX)
  if (!chrome::GetBrowserCount(profile_)) {
    TabRestoreServiceFactory::ResetForProfile(profile_);
  }
#endif

  profile_pref_registrar_.RemoveAll();

  encoding_auto_detect_.Destroy();

  command_controller_.reset();

  if (profile_->IsOffTheRecord() &&
      !BrowserList::IsOffTheRecordSessionActiveForProfile(profile_)) {
    ProfileDestroyer::DestroyProfileWhenAppropriate(profile_);
  }

  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();
}
