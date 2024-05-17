  void NotifyRefreshIME() {
    ash::IMEObserver* observer = tray_->ime_observer();
    if (observer)
      observer->OnIMERefresh();
  }
