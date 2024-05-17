  ~SendThumbnailTask() {
    if (ready_)
      ready_->Signal();
  }
