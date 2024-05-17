void WebBluetoothServiceImpl::RemoteCharacteristicReadValue(
    const std::string& characteristic_instance_id,
    RemoteCharacteristicReadValueCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  const CacheQueryResult query_result =
      QueryCacheForCharacteristic(characteristic_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordCharacteristicReadValueOutcome(query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult(),
                            base::nullopt  );
    return;
  }

  if (BluetoothBlocklist::Get().IsExcludedFromReads(
          query_result.characteristic->GetUUID())) {
    RecordCharacteristicReadValueOutcome(UMAGATTOperationOutcome::BLOCKLISTED);
    std::move(callback).Run(blink::mojom::WebBluetoothResult::BLOCKLISTED_READ,
                            base::nullopt  );
    return;
  }

  auto copyable_callback = AdaptCallbackForRepeating(std::move(callback));
  query_result.characteristic->ReadRemoteCharacteristic(
      base::Bind(&WebBluetoothServiceImpl::OnCharacteristicReadValueSuccess,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback),
      base::Bind(&WebBluetoothServiceImpl::OnCharacteristicReadValueFailed,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback));
}
