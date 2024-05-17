void DownloadCoreServiceImpl::Shutdown() {
  if (download_manager_created_) {
    BrowserContext::GetDownloadManager(profile_)->Shutdown();
  }
#if BUILDFLAG(ENABLE_EXTENSIONS)
  extension_event_router_.reset();
#endif
  manager_delegate_.reset();
  download_history_.reset();
}
