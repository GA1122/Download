bool BluetoothDeviceChromeOS::IsConnecting() const {
  return num_connecting_calls_ > 0;
}
