void BluetoothChooserDesktop::AddOrUpdateDevice(
    const std::string& device_id,
    bool should_update_name,
    const base::string16& device_name,
    bool is_gatt_connected,
    bool is_paired,
    int signal_strength_level) {
  bluetooth_chooser_controller_->AddOrUpdateDevice(
      device_id, should_update_name, device_name, is_gatt_connected, is_paired,
      signal_strength_level);
}
