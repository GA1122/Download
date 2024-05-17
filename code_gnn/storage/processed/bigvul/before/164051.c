void DownloadManagerImpl::OnDownloadsInitialized() {
  in_progress_downloads_ = in_progress_manager_->TakeInProgressDownloads();
  uint32_t max_id = download::DownloadItem::kInvalidId;
  for (auto it = in_progress_downloads_.begin();
       it != in_progress_downloads_.end();) {
    download::DownloadItemImpl* download = it->get();
    uint32_t id = download->GetId();
    if (id > max_id)
      max_id = id;
#if defined(OS_ANDROID)
    if (ShouldClearDownloadFromDB(download->GetURL(), download->GetState(),
                                  download->GetLastReason())) {
      cleared_download_guids_on_startup_.insert(download->GetGuid());
      DeleteDownloadedFileOnUIThread(download->GetFullPath());
      it = in_progress_downloads_.erase(it);
      continue;
    }
#endif   
    ++it;
  }
  PostInitialization(DOWNLOAD_INITIALIZATION_DEPENDENCY_IN_PROGRESS_CACHE);
  SetNextId(max_id + 1);
}
