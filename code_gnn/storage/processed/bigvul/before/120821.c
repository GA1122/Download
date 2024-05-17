void BluetoothDeviceChromeOS::OnSetTrusted(bool success) {
  LOG_IF(WARNING, !success) << object_path_.value()
                             << ": Failed to set device as trusted";
 }
