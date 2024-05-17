void WebBluetoothServiceImpl::RemoteServerGetPrimaryServices(
    const blink::WebBluetoothDeviceId& device_id,
    blink::mojom::WebBluetoothGATTQueryQuantity quantity,
    const base::Optional<BluetoothUUID>& services_uuid,
    RemoteServerGetPrimaryServicesCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RecordGetPrimaryServicesServices(quantity, services_uuid);

  if (!allowed_devices().IsAllowedToAccessAtLeastOneService(device_id)) {
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::NOT_ALLOWED_TO_ACCESS_ANY_SERVICE,
        base::nullopt  );
    return;
  }

  if (services_uuid && !allowed_devices().IsAllowedToAccessService(
                           device_id, services_uuid.value())) {
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::NOT_ALLOWED_TO_ACCESS_SERVICE,
        base::nullopt  );
    return;
  }

  const CacheQueryResult query_result = QueryCacheForDevice(device_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordGetPrimaryServicesOutcome(quantity, query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult(),
                            base::nullopt  );
    return;
  }

  const std::string& device_address = query_result.device->GetAddress();

  if (query_result.device->IsGattServicesDiscoveryComplete()) {
    RemoteServerGetPrimaryServicesImpl(device_id, quantity, services_uuid,
                                       std::move(callback),
                                       query_result.device);
    return;
  }

  DVLOG(1) << "Services not yet discovered.";
  pending_primary_services_requests_[device_address].push_back(base::BindOnce(
      &WebBluetoothServiceImpl::RemoteServerGetPrimaryServicesImpl,
      base::Unretained(this), device_id, quantity, services_uuid,
      std::move(callback)));
}
