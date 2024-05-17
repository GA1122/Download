void WebContentsImpl::DecrementBluetoothConnectedDeviceCount() {
  if (IsBeingDestroyed()) {
    return;
  }
  DCHECK(bluetooth_connected_device_count_ != 0);
  bluetooth_connected_device_count_--;
  if (bluetooth_connected_device_count_ == 0) {
    NotifyNavigationStateChanged(INVALIDATE_TYPE_TAB);
  }
}
