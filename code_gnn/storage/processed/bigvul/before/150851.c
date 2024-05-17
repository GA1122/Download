BluetoothAdapter::ConstDeviceList BluetoothAdapter::GetDevices() const {
  ConstDeviceList devices;
  for (const auto& device : devices_)
    devices.push_back(device.second.get());

  return devices;
}
