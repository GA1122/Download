void DownloadManagerImpl::OnDownloadManagerInitialized() {
  OnInitialized();
  in_progress_manager_->OnAllInprogressDownloadsLoaded();
  for (auto& observer : observers_)
    observer.OnManagerInitialized();
}
