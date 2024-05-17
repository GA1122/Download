  void UpdateSystemInfo() {
    if (EnsureCrosLoaded()) {
      UpdateNetworkManagerStatus();
    }
  }
