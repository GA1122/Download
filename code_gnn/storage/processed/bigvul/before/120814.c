void BluetoothDeviceChromeOS::OnCancelPairingError(
    const std::string& error_name,
    const std::string& error_message) {
  LOG(WARNING) << object_path_.value() << ": Failed to cancel pairing: "
               << error_name << ": " << error_message;
}
