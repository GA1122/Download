bool WebContentsImpl::IsConnectedToBluetoothDevice() const {
  return bluetooth_connected_device_count_ > 0;
}
