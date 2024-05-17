  FileMetricsProvider* provider() {
    if (!provider_)
      provider_.reset(new FileMetricsProvider(prefs()));
    return provider_.get();
  }
