  void Add(const std::string& event) {
    base::AutoLock lock(lock_);
    log_ += event + "\n";
  }
