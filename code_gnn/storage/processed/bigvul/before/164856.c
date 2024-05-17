  explicit PercentWaiter(DownloadItem* item) : item_(item) {
    item_->AddObserver(this);
  }
