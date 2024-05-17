uint16 BluetoothDeviceChromeOS::GetDeviceID() const {
  uint16 device_id  = 0;
  ParseModalias(object_path_, NULL, NULL, &device_id);
  return device_id;
}
