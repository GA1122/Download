  void Wait() {
    std::vector<DownloadItem*> downloads;
    manager_->GetAllDownloads(&downloads);
    if (!downloads.empty())
      return;
    waiting_ = true;
    content::RunMessageLoop();
    waiting_ = false;
  }
