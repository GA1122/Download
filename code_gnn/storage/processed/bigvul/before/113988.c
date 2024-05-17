  void NotifyRefreshDrive(ash::DriveOperationStatusList& list) {
    ash::DriveObserver* observer = tray_->drive_observer();
    if (observer)
      observer->OnDriveRefresh(list);
  }
