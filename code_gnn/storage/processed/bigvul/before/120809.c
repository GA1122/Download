uint16 BluetoothDeviceChromeOS::GetVendorID() const {
  uint16 vendor_id  = 0;
  ParseModalias(object_path_, &vendor_id, NULL, NULL);
  return vendor_id;
}
