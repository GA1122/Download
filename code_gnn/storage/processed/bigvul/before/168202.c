void WebBluetoothServiceImpl::OnGetDeviceSuccess(
    RequestDeviceCallback callback,
    blink::mojom::WebBluetoothRequestDeviceOptionsPtr options,
    const std::string& device_address) {
  device_chooser_controller_.reset();

  const device::BluetoothDevice* const device =
      GetAdapter()->GetDevice(device_address);
  if (device == nullptr) {
    DVLOG(1) << "Device " << device_address << " no longer in adapter";
    RecordRequestDeviceOutcome(UMARequestDeviceOutcome::CHOSEN_DEVICE_VANISHED);
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::CHOSEN_DEVICE_VANISHED,
        nullptr  );
    return;
  }

  const WebBluetoothDeviceId device_id =
      allowed_devices().AddDevice(device_address, options);

  DVLOG(1) << "Device: " << device->GetNameForDisplay();

  blink::mojom::WebBluetoothDevicePtr device_ptr =
      blink::mojom::WebBluetoothDevice::New();
  device_ptr->id = device_id;
  device_ptr->name = device->GetName();

  RecordRequestDeviceOutcome(UMARequestDeviceOutcome::SUCCESS);
  std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS,
                          std::move(device_ptr));
}
