bool BluetoothAdapterChromeOS::IsPresent() const {
  return !object_path_.value().empty();
}
