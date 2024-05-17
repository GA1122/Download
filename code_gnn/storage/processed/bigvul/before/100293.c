  virtual void RequestWifiScan() {
    if (EnsureCrosLoaded()) {
      RequestScan(TYPE_WIFI);
    }
  }
