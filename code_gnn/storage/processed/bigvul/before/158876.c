Browser::~Browser() {
  registrar_.RemoveAll();
  extension_registry_observer_.RemoveAll();

  DCHECK(tab_strip_model_->empty());
  tab_strip_model_->RemoveObserver(this);
  bubble_manager_.reset();

  command_controller_.reset();
  BrowserList::RemoveBrowser(this);

  int num_downloads;
  if (!browser_defaults::kBrowserAliveWithNoWindows &&
      OkToCloseWithInProgressDownloads(&num_downloads) ==
          DOWNLOAD_CLOSE_BROWSER_SHUTDOWN) {
    DownloadCoreService::CancelAllDownloads();
  }

  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service)
    session_service->WindowClosed(session_id_);

  sessions::TabRestoreService* tab_restore_service =
      TabRestoreServiceFactory::GetForProfile(profile());
  if (tab_restore_service)
    tab_restore_service->BrowserClosed(live_tab_context());

  profile_pref_registrar_.RemoveAll();

  extension_window_controller_.reset();

  instant_controller_.reset();

  if (profile_->IsOffTheRecord() &&
      profile_->GetOriginalProfile()->HasOffTheRecordProfile() &&
      profile_->GetOriginalProfile()->GetOffTheRecordProfile() == profile_ &&
      !BrowserList::IsIncognitoSessionActiveForProfile(profile_) &&
      !profile_->GetOriginalProfile()->IsSystemProfile()) {
    if (profile_->IsGuestSession()) {
#if !defined(OS_CHROMEOS)
      profiles::RemoveBrowsingDataForProfile(profile_->GetPath());
#endif
    } else {
#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
      g_browser_process->background_printing_manager()
          ->DeletePreviewContentsForBrowserContext(profile_);
#endif
      ProfileDestroyer::DestroyProfileWhenAppropriate(profile_);
    }
  }

  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();
}
