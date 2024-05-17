  void Cancel() {
    base::AutoLock locked(lock_);
    cancelled_ = true;
  }
