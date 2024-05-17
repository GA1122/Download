void WebBluetoothServiceImpl::RemoteCharacteristicGetDescriptors(
    const std::string& characteristic_instance_id,
    blink::mojom::WebBluetoothGATTQueryQuantity quantity,
    const base::Optional<BluetoothUUID>& descriptors_uuid,
    RemoteCharacteristicGetDescriptorsCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  RecordGetDescriptorsDescriptor(quantity, descriptors_uuid);

  if (descriptors_uuid &&
      BluetoothBlocklist::Get().IsExcluded(descriptors_uuid.value())) {
    RecordGetDescriptorsOutcome(quantity, UMAGetDescriptorOutcome::BLOCKLISTED);
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::BLOCKLISTED_DESCRIPTOR_UUID,
        base::nullopt  );
    return;
  }

  const CacheQueryResult query_result =
      QueryCacheForCharacteristic(characteristic_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordGetDescriptorsOutcome(quantity, query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult(),
                            base::nullopt  );
    return;
  }

  auto descriptors = descriptors_uuid
                         ? query_result.characteristic->GetDescriptorsByUUID(
                               descriptors_uuid.value())
                         : query_result.characteristic->GetDescriptors();

  std::vector<blink::mojom::WebBluetoothRemoteGATTDescriptorPtr>
      response_descriptors;
  for (device::BluetoothRemoteGattDescriptor* descriptor : descriptors) {
    if (BluetoothBlocklist::Get().IsExcluded(descriptor->GetUUID())) {
      continue;
    }
    std::string descriptor_instance_id = descriptor->GetIdentifier();
    auto insert_result = descriptor_id_to_characteristic_id_.insert(
        {descriptor_instance_id, characteristic_instance_id});
    if (!insert_result.second)
      DCHECK(insert_result.first->second == characteristic_instance_id);

    auto descriptor_ptr(blink::mojom::WebBluetoothRemoteGATTDescriptor::New());
    descriptor_ptr->instance_id = descriptor_instance_id;
    descriptor_ptr->uuid = descriptor->GetUUID();
    response_descriptors.push_back(std::move(descriptor_ptr));

    if (quantity == blink::mojom::WebBluetoothGATTQueryQuantity::SINGLE) {
      break;
    }
  }

  if (!response_descriptors.empty()) {
    RecordGetDescriptorsOutcome(quantity, UMAGetDescriptorOutcome::SUCCESS);
    std::move(callback).Run(blink::mojom::WebBluetoothResult::SUCCESS,
                            std::move(response_descriptors));
    return;
  }
  RecordGetDescriptorsOutcome(
      quantity, descriptors_uuid ? UMAGetDescriptorOutcome::NOT_FOUND
                                 : UMAGetDescriptorOutcome::NO_DESCRIPTORS);
  std::move(callback).Run(
      descriptors_uuid ? blink::mojom::WebBluetoothResult::DESCRIPTOR_NOT_FOUND
                       : blink::mojom::WebBluetoothResult::NO_DESCRIPTORS_FOUND,
      base::nullopt  );
}
