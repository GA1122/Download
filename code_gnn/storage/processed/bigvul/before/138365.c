  void ClearConnectionFiltersOnIOThread() {
    base::AutoLock lock(lock_);
    connection_filters_.clear();
  }
