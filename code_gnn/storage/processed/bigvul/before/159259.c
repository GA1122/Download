void InProgressDownloadObserver::OnDownloadRemoved(DownloadItem* download) {
  if (!in_progress_cache_)
    return;

  in_progress_cache_->RemoveEntry(download->GetGuid());
}
