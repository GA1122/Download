void WebBluetoothServiceImpl::RequestScanningStart(
    blink::mojom::WebBluetoothScanClientAssociatedPtrInfo client_info,
    blink::mojom::WebBluetoothRequestLEScanOptionsPtr options,
    RequestScanningStartCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  blink::mojom::WebBluetoothScanClientAssociatedPtr client;
  client.Bind(std::move(client_info));

  if (!GetAdapter()) {
    if (BluetoothAdapterFactoryWrapper::Get().IsLowEnergySupported()) {
      BluetoothAdapterFactoryWrapper::Get().AcquireAdapter(
          this,
          base::BindOnce(&WebBluetoothServiceImpl::RequestScanningStartImpl,
                         weak_ptr_factory_.GetWeakPtr(), std::move(client),
                         std::move(options), std::move(callback)));
      return;
    }
    auto result = blink::mojom::RequestScanningStartResult::NewErrorResult(
        blink::mojom::WebBluetoothResult::BLUETOOTH_LOW_ENERGY_NOT_AVAILABLE);
    std::move(callback).Run(std::move(result));
    return;
  }

  RequestScanningStartImpl(std::move(client), std::move(options),
                           std::move(callback), GetAdapter());
}
