  std::string GetAndReset() {
    base::AutoLock lock(lock_);
    return std::move(log_);
  }
