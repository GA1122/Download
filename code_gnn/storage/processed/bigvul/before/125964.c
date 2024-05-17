void AllDownloadsCompleteObserver::ManagerGoingDown(DownloadManager* manager) {
  DCHECK_EQ(manager, download_manager_);
  download_manager_->RemoveObserver(this);
  download_manager_ = NULL;
}
