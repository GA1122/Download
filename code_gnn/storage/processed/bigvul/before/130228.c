  ~ScopedClipboard() {
    if (opened_) {
      ::CloseClipboard();
    }
  }
