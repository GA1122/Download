  void ShutDownOnIOThread() {
    service_manager_.reset();
    manifest_provider_.reset();
  }
