void DownloadManagerImpl::PostInitialization(
    DownloadInitializationDependency dependency) {
  if (initialized_)
    return;

  switch (dependency) {
    case DOWNLOAD_INITIALIZATION_DEPENDENCY_HISTORY_DB:
      history_db_initialized_ = true;
      break;
    case DOWNLOAD_INITIALIZATION_DEPENDENCY_IN_PROGRESS_CACHE:
      in_progress_cache_initialized_ = true;
      if (load_history_downloads_cb_) {
        base::ThreadTaskRunnerHandle::Get()->PostTask(
            FROM_HERE, std::move(load_history_downloads_cb_));
      }
      break;
    case DOWNLOAD_INITIALIZATION_DEPENDENCY_NONE:
    default:
      NOTREACHED();
      break;
  }

  initialized_ = history_db_initialized_ && in_progress_cache_initialized_;

  if (!initialized_)
    return;

#if defined(OS_ANDROID)
    for (const auto& guid : cleared_download_guids_on_startup_)
      in_progress_manager_->RemoveInProgressDownload(guid);
    if (cancelled_download_cleared_from_history_ > 0) {
      UMA_HISTOGRAM_COUNTS_1000(
          "MobileDownload.CancelledDownloadRemovedFromHistory",
          cancelled_download_cleared_from_history_);
    }

    if (interrupted_download_cleared_from_history_ > 0) {
      UMA_HISTOGRAM_COUNTS_1000(
          "MobileDownload.InterruptedDownloadsRemovedFromHistory",
          interrupted_download_cleared_from_history_);
    }
#endif

    for (auto& download : in_progress_downloads_) {
      auto item = std::move(download.second);
      item->SetDelegate(this);
      DownloadItemUtils::AttachInfo(item.get(), GetBrowserContext(), nullptr);
      OnDownloadCreated(std::move(item));
    }
    in_progress_downloads_.clear();

    in_progress_manager_->OnAllInprogressDownloadsLoaded();
    for (auto& observer : observers_)
      observer.OnManagerInitialized();
}
