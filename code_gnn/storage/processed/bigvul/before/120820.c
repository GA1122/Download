void BluetoothDeviceChromeOS::OnForgetError(
    const ErrorCallback& error_callback,
    const std::string& error_name,
    const std::string& error_message) {
  LOG(WARNING) << object_path_.value() << ": Failed to remove device: "
               << error_name << ": " << error_message;
   error_callback.Run();
 }
