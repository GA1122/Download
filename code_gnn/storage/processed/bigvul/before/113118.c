    explicit MockObserver(DownloadItem* item) : item_(item), updated_(false) {
      item_->AddObserver(this);
    }
