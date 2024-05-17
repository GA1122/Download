  explicit CreatedObserver(content::DownloadManager* manager)
      : manager_(manager),
        waiting_(false) {
    manager->AddObserver(this);
  }
