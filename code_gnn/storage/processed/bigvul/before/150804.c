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

  const blink::WebBluetoothDeviceId device_id =
      allowed_devices().AddDevice(device_address, options);

  DVLOG(1) << "Device: " << device->GetNameForDisplay();

  auto web_bluetooth_device = blink::mojom::WebBluetoothDevice::New();
  web_bluetooth_device->id = device_id;
  web_bluetooth_device->name = device->GetName();

  RecordRequestDeviceOutcome(UMARequestDeviceOutcome::SUCCESS);
  std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS,
                          std::move(web_bluetooth_device));
}
