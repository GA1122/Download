void WebBluetoothServiceImpl::RequestDevice(
    blink::mojom::WebBluetoothRequestDeviceOptionsPtr options,
    RequestDeviceCallback callback) {
  RecordRequestDeviceOptions(options);

  if (!GetAdapter()) {
    if (BluetoothAdapterFactoryWrapper::Get().IsLowEnergySupported()) {
      BluetoothAdapterFactoryWrapper::Get().AcquireAdapter(
          this, base::Bind(&WebBluetoothServiceImpl::RequestDeviceImpl,
                           weak_ptr_factory_.GetWeakPtr(),
                           base::Passed(&options), base::Passed(&callback)));
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
