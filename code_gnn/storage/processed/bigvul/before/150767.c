void WebBluetoothServiceImpl::ScanningClient::AddFilteredDeviceToPrompt(
    const std::string& device_id,
    const base::Optional<std::string>& device_name) {
  bool should_update_name = device_name.has_value();
  base::string16 device_name_for_display =
      base::UTF8ToUTF16(device_name.value_or(""));
  prompt_controller_->AddFilteredDevice(device_id, should_update_name,
                                        device_name_for_display);
}
