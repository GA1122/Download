void BluetoothAdapterChromeOS::OnStartDiscoveryError(
    const ErrorCallback& error_callback,
    const std::string& error_name,
    const std::string& error_message) {
  LOG(WARNING) << object_path_.value() << ": Failed to start discovery: "
               << error_name << ": " << error_message;
  error_callback.Run();
}
