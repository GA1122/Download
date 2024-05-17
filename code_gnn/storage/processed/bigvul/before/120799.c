void BluetoothDeviceChromeOS::ConnectToService(
    const std::string& service_uuid,
    const SocketCallback& callback) {
  callback.Run(scoped_refptr<device::BluetoothSocket>());
}
