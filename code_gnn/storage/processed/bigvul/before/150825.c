void WebBluetoothServiceImpl::RemoteServiceGetCharacteristics(
    const std::string& service_instance_id,
    blink::mojom::WebBluetoothGATTQueryQuantity quantity,
    const base::Optional<BluetoothUUID>& characteristics_uuid,
    RemoteServiceGetCharacteristicsCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  RecordGetCharacteristicsCharacteristic(quantity, characteristics_uuid);

  if (characteristics_uuid &&
      BluetoothBlocklist::Get().IsExcluded(characteristics_uuid.value())) {
    RecordGetCharacteristicsOutcome(quantity,
                                    UMAGetCharacteristicOutcome::BLOCKLISTED);
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::BLOCKLISTED_CHARACTERISTIC_UUID,
        base::nullopt  );
    return;
  }

  const CacheQueryResult query_result =
      QueryCacheForService(service_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordGetCharacteristicsOutcome(quantity, query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult(),
                            base::nullopt  );
    return;
  }

  std::vector<device::BluetoothRemoteGattCharacteristic*> characteristics =
      characteristics_uuid ? query_result.service->GetCharacteristicsByUUID(
                                 characteristics_uuid.value())
                           : query_result.service->GetCharacteristics();

  std::vector<blink::mojom::WebBluetoothRemoteGATTCharacteristicPtr>
      response_characteristics;
  for (device::BluetoothRemoteGattCharacteristic* characteristic :
       characteristics) {
    if (BluetoothBlocklist::Get().IsExcluded(characteristic->GetUUID())) {
      continue;
    }
    std::string characteristic_instance_id = characteristic->GetIdentifier();
    auto insert_result = characteristic_id_to_service_id_.insert(
        std::make_pair(characteristic_instance_id, service_instance_id));
    if (!insert_result.second)
      DCHECK(insert_result.first->second == service_instance_id);

    blink::mojom::WebBluetoothRemoteGATTCharacteristicPtr characteristic_ptr =
        blink::mojom::WebBluetoothRemoteGATTCharacteristic::New();
    characteristic_ptr->instance_id = characteristic_instance_id;
    characteristic_ptr->uuid = characteristic->GetUUID();
    characteristic_ptr->properties =
        static_cast<uint32_t>(characteristic->GetProperties());
    response_characteristics.push_back(std::move(characteristic_ptr));

    if (quantity == blink::mojom::WebBluetoothGATTQueryQuantity::SINGLE) {
      break;
    }
  }

  if (!response_characteristics.empty()) {
    RecordGetCharacteristicsOutcome(quantity,
                                    UMAGetCharacteristicOutcome::SUCCESS);
    std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS,
                            std::move(response_characteristics));
    return;
  }

  RecordGetCharacteristicsOutcome(
      quantity, characteristics_uuid
                    ? UMAGetCharacteristicOutcome::NOT_FOUND
                    : UMAGetCharacteristicOutcome::NO_CHARACTERISTICS);
  std::move(callback).Run(
      characteristics_uuid
          ? blink::mojom::WebBluetoothResult::CHARACTERISTIC_NOT_FOUND
          : blink::mojom::WebBluetoothResult::NO_CHARACTERISTICS_FOUND,
      base::nullopt  );
}
