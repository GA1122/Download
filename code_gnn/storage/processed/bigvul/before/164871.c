  bool WaitForFinished() {
    if (item_->GetState() == DownloadItem::COMPLETE) {
      return item_->PercentComplete() == 100;
    }
    waiting_ = true;
    content::RunMessageLoop();
    waiting_ = false;
    return !error_;
  }
