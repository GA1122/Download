void DownloadManagerImpl::OnInProgressDownloadManagerInitialized() {
  std::vector<std::unique_ptr<download::DownloadItemImpl>>
      in_progress_downloads = in_progress_manager_->TakeInProgressDownloads();
  uint32_t max_id = download::DownloadItem::kInvalidId;
  for (auto& download : in_progress_downloads) {
    uint32_t id = download->GetId();
    if (base::ContainsKey(in_progress_downloads_, id)) {
      in_progress_manager_->RemoveInProgressDownload(download->GetGuid());
      continue;
    }
    if (id > max_id)
      max_id = id;
#if defined(OS_ANDROID)
    if (ShouldClearDownloadFromDB(download->GetURL(), download->GetState(),
                                  download->GetLastReason())) {
      cleared_download_guids_on_startup_.insert(download->GetGuid());
      DeleteDownloadedFileOnUIThread(download->GetFullPath());
      continue;
    }
#endif   
    in_progress_downloads_[id] = std::move(download);
  }
  PostInitialization(DOWNLOAD_INITIALIZATION_DEPENDENCY_IN_PROGRESS_CACHE);
  SetNextId(max_id + 1);
}
