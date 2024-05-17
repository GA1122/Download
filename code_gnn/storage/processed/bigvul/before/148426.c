void WebContentsImpl::IncrementBluetoothConnectedDeviceCount() {
  if (IsBeingDestroyed()) {
    return;
  }
  bluetooth_connected_device_count_++;
  if (bluetooth_connected_device_count_ == 1) {
    NotifyNavigationStateChanged(INVALIDATE_TYPE_TAB);
  }
}
