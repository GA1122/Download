  void CancelLock() {
    if (!root_window_)
      return;
    UnlockCompositor();
    root_window_->ReleaseMouseMoves();
    root_window_ = NULL;
  }
