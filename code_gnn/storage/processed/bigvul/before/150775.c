void WebBluetoothServiceImpl::DeviceChanged(device::BluetoothAdapter* adapter,
                                            device::BluetoothDevice* device) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (device_chooser_controller_.get()) {
    device_chooser_controller_->AddFilteredDevice(*device);
  }

  if (!device->IsGattConnected()) {
    base::Optional<blink::WebBluetoothDeviceId> device_id =
        connected_devices_->CloseConnectionToDeviceWithAddress(
            device->GetAddress());

    RunPendingPrimaryServicesRequests(device);
  }
}
