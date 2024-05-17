  void Disable() {
    base::AutoLock lock(enabled_lock_);
    enabled_ = false;
  }
