void VirtualKeyboardController::UpdateDevices() {
  ui::DeviceDataManager* device_data_manager =
      ui::DeviceDataManager::GetInstance();

  has_touchscreen_ = device_data_manager->touchscreen_devices().size() > 0;

  has_external_keyboard_ = false;
  has_internal_keyboard_ = false;
  for (const ui::KeyboardDevice& device :
       device_data_manager->keyboard_devices()) {
    if (has_internal_keyboard_ && has_external_keyboard_)
      break;
    ui::InputDeviceType type = device.type;
    if (type == ui::InputDeviceType::INPUT_DEVICE_INTERNAL)
      has_internal_keyboard_ = true;
    if (type == ui::InputDeviceType::INPUT_DEVICE_EXTERNAL)
      has_external_keyboard_ = true;
  }
  UpdateKeyboardEnabled();
}
