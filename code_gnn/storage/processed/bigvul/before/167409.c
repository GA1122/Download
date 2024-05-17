int DownloadCoreServiceImpl::NonMaliciousDownloadCount() const {
  if (!download_manager_created_)
    return 0;
  return BrowserContext::GetDownloadManager(profile_)
      ->NonMaliciousInProgressCount();
}
