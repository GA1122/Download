void BluetoothDeviceChromeOS::ProvidesServiceWithName(
    const std::string& name,
    const ProvidesServiceCallback& callback) {
  callback.Run(false);
 }
