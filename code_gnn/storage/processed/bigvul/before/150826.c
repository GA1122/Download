 void WebBluetoothServiceImpl::RequestDevice(
     blink::mojom::WebBluetoothRequestDeviceOptionsPtr options,
     RequestDeviceCallback callback) {
  RecordRequestDeviceOptions(options);

  if (!GetAdapter()) {
    if (BluetoothAdapterFactoryWrapper::Get().IsLowEnergySupported()) {
      BluetoothAdapterFactoryWrapper::Get().AcquireAdapter(
          this, base::BindOnce(&WebBluetoothServiceImpl::RequestDeviceImpl,
                               weak_ptr_factory_.GetWeakPtr(),
                               std::move(options), std::move(callback)));
      return;
    }
    RecordRequestDeviceOutcome(
        UMARequestDeviceOutcome::BLUETOOTH_LOW_ENERGY_NOT_AVAILABLE);
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::BLUETOOTH_LOW_ENERGY_NOT_AVAILABLE,
        nullptr  );
    return;
  }
  RequestDeviceImpl(std::move(options), std::move(callback), GetAdapter());
}
