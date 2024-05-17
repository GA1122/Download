void BluetoothDeviceChromeOS::OnConnectProfileError(
    device::BluetoothProfile* profile,
    const ErrorCallback& error_callback,
    const std::string& error_name,
    const std::string& error_message) {
  BluetoothProfileChromeOS* profile_chromeos =
      static_cast<BluetoothProfileChromeOS*>(profile);
  VLOG(1) << object_path_.value() << ": Profile connection failed: "
          << profile_chromeos->uuid() << ": "
          << error_name << ": " << error_message;
  error_callback.Run();
}
