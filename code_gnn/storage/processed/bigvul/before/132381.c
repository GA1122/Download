  void UpdateTouchscreenDevices(
      std::vector<ui::TouchscreenDevice> touchscreen_devices) {
    ui::DeviceHotplugEventObserver* manager =
        ui::DeviceDataManager::GetInstance();
    manager->OnTouchscreenDevicesUpdated(touchscreen_devices);
  }
