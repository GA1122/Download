  void StopWorkerThread() {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    thread_.reset();
  }
