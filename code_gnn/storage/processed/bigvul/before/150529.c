  void SetLocationHeader(const std::string& header) {
    base::AutoLock auto_lock(lock_);
    location_header_ = header;
  }
