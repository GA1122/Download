  ~DownloadObserver() {
    download_manager_->RemoveObserver(this);
  }
