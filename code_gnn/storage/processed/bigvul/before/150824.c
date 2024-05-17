void WebBluetoothServiceImpl::RemoteServerGetPrimaryServicesImpl(
    const blink::WebBluetoothDeviceId& device_id,
    blink::mojom::WebBluetoothGATTQueryQuantity quantity,
    const base::Optional<BluetoothUUID>& services_uuid,
    RemoteServerGetPrimaryServicesCallback callback,
    device::BluetoothDevice* device) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!device->IsGattConnected()) {
    RecordGetPrimaryServicesOutcome(
        quantity, UMAGetPrimaryServiceOutcome::DEVICE_DISCONNECTED);
    std::move(callback).Run(blink::mojom::WebBluetoothResult::NO_SERVICES_FOUND,
                            base::nullopt  );
    return;
  }

  DCHECK(device->IsGattServicesDiscoveryComplete());

  std::vector<device::BluetoothRemoteGattService*> services =
      services_uuid ? device->GetPrimaryServicesByUUID(services_uuid.value())
                    : device->GetPrimaryServices();

  std::vector<blink::mojom::WebBluetoothRemoteGATTServicePtr> response_services;
  for (device::BluetoothRemoteGattService* service : services) {
    if (!allowed_devices().IsAllowedToAccessService(device_id,
                                                    service->GetUUID())) {
      continue;
    }
    std::string service_instance_id = service->GetIdentifier();
    const std::string& device_address = device->GetAddress();
    auto insert_result = service_id_to_device_address_.insert(
        make_pair(service_instance_id, device_address));
    if (!insert_result.second)
      DCHECK_EQ(insert_result.first->second, device_address);

    blink::mojom::WebBluetoothRemoteGATTServicePtr service_ptr =
        blink::mojom::WebBluetoothRemoteGATTService::New();
    service_ptr->instance_id = service_instance_id;
    service_ptr->uuid = service->GetUUID();
    response_services.push_back(std::move(service_ptr));

    if (quantity == blink::mojom::WebBluetoothGATTQueryQuantity::SINGLE) {
      break;
    }
  }

  if (!response_services.empty()) {
    DVLOG(1) << "Services found in device.";
    RecordGetPrimaryServicesOutcome(quantity,
                                    UMAGetPrimaryServiceOutcome::SUCCESS);
    std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS,
                            std::move(response_services));
    return;
  }

  DVLOG(1) << "Services not found in device.";
  RecordGetPrimaryServicesOutcome(
      quantity, services_uuid ? UMAGetPrimaryServiceOutcome::NOT_FOUND
                              : UMAGetPrimaryServiceOutcome::NO_SERVICES);
  std::move(callback).Run(
      services_uuid ? blink::mojom::WebBluetoothResult::SERVICE_NOT_FOUND
                    : blink::mojom::WebBluetoothResult::NO_SERVICES_FOUND,
      base::nullopt  );
}
