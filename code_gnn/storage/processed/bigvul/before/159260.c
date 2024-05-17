void InProgressDownloadObserver::OnDownloadUpdated(DownloadItem* download) {
  if (!in_progress_cache_)
    return;

  switch (download->GetState()) {
    case DownloadItem::DownloadState::COMPLETE:
    case DownloadItem::DownloadState::CANCELLED:
      if (in_progress_cache_)
        in_progress_cache_->RemoveEntry(download->GetGuid());
      break;
    case DownloadItem::DownloadState::IN_PROGRESS:
      break;
    default:
      break;
  }
}
