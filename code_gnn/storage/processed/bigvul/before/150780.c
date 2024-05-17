void WebBluetoothServiceImpl::GattServicesDiscovered(
    device::BluetoothAdapter* adapter,
    device::BluetoothDevice* device) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  DVLOG(1) << "Services discovered for device: " << device->GetAddress();

  if (device_chooser_controller_.get()) {
    device_chooser_controller_->AddFilteredDevice(*device);
  }

  RunPendingPrimaryServicesRequests(device);
}
