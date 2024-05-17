void BluetoothDeviceChooserController::AddFilteredDevice(
    const device::BluetoothDevice& device) {
  base::Optional<std::string> device_name = device.GetName();
  if (chooser_.get()) {
    if (options_->accept_all_devices ||
        MatchesFilters(device_name ? &device_name.value() : nullptr,
                       device.GetUUIDs(), options_->filters)) {
      base::Optional<int8_t> rssi = device.GetInquiryRSSI();
      std::string device_id = device.GetAddress();
      device_ids_.insert(device_id);
      chooser_->AddOrUpdateDevice(
          device_id, !!device.GetName()  ,
          device.GetNameForDisplay(), device.IsGattConnected(),
          web_bluetooth_service_->IsDevicePaired(device.GetAddress()),
          rssi ? CalculateSignalStrengthLevel(rssi.value()) : -1);
    }
  }
}
