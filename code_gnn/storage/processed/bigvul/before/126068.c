AllDownloadsCompleteObserver::~AllDownloadsCompleteObserver() {
  if (download_manager_) {
    download_manager_->RemoveObserver(this);
    download_manager_ = NULL;
  }
  for (std::set<DownloadItem*>::const_iterator it = pending_downloads_.begin();
       it != pending_downloads_.end(); ++it) {
    (*it)->RemoveObserver(this);
  }
  pending_downloads_.clear();
}
