void WebBluetoothServiceImpl::DeviceAdded(device::BluetoothAdapter* adapter,
                                          device::BluetoothDevice* device) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (device_chooser_controller_.get()) {
    device_chooser_controller_->AddFilteredDevice(*device);
  }
}
