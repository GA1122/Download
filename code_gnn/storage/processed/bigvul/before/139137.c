  void Detach() {
    base::AutoLock lock(lock_);
    filter_ = nullptr;
  }
