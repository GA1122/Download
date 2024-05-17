void AllDownloadsCompleteObserver::OnDownloadUpdated(DownloadItem* download) {
  if (download->GetState() != DownloadItem::IN_PROGRESS) {
    download->RemoveObserver(this);
    pending_downloads_.erase(download);
    ReplyIfNecessary();
  }
}
