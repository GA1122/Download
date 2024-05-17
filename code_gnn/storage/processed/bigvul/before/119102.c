  void UnregisterCallback() {
    if (is_initialized_) {
      google::InstallSymbolizeOpenObjectFileCallback(NULL);
      is_initialized_ = false;
    }
  }
