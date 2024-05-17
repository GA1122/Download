  void NotifyRefreshBluetooth() {
    ash::BluetoothObserver* observer = tray_->bluetooth_observer();
    if (observer)
      observer->OnBluetoothRefresh();
  }
