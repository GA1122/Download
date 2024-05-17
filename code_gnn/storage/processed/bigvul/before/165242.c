  void CallHandleEnabledStateChange(bool enabled) {
    is_enabled_ = enabled;
    client_->HandleEnabledStateChange(enabled);
  }
