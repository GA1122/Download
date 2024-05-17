  void SetState(DownloadState state) {
    state_ = state;
    UpdateObservers();
  }
