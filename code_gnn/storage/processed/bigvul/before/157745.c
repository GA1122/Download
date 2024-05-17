void WebContentsImpl::DecrementBluetoothConnectedDeviceCount() {
  if (IsBeingDestroyed()) {
    return;
  }
  DCHECK_NE(bluetooth_connected_device_count_, 0u);
  bluetooth_connected_device_count_--;
  if (bluetooth_connected_device_count_ == 0) {
    NotifyNavigationStateChanged(INVALIDATE_TYPE_TAB);
  }
}
