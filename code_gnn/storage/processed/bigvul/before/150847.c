void BluetoothAdapter::DeleteDeviceForTesting(const std::string& address) {
  devices_.erase(address);
}
