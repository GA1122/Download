  explicit DefaultBrowserActionRecorder(base::Closure continuation)
      : continuation_(std::move(continuation)), settings_app_monitor_(this) {}
