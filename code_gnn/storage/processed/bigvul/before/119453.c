void ScreenOrientationDispatcherHost::ResetCurrentLock() {
  if (current_lock_) {
    delete current_lock_;
    current_lock_ = 0;
  }
}
