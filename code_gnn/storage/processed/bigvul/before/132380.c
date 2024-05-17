  void UpdateKeyboardDevices(std::vector<ui::KeyboardDevice> keyboard_devices) {
    ui::DeviceHotplugEventObserver* manager =
        ui::DeviceDataManager::GetInstance();
    manager->OnKeyboardDevicesUpdated(keyboard_devices);
  }
