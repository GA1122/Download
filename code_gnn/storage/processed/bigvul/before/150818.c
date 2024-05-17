void WebBluetoothServiceImpl::RemoteCharacteristicWriteValue(
    const std::string& characteristic_instance_id,
    const std::vector<uint8_t>& value,
    RemoteCharacteristicWriteValueCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (value.size() > 512) {
    CrashRendererAndClosePipe(bad_message::BDH_INVALID_WRITE_VALUE_LENGTH);
    return;
  }

  const CacheQueryResult query_result =
      QueryCacheForCharacteristic(characteristic_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordCharacteristicWriteValueOutcome(query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult());
    return;
  }

  if (BluetoothBlocklist::Get().IsExcludedFromWrites(
          query_result.characteristic->GetUUID())) {
    RecordCharacteristicWriteValueOutcome(UMAGATTOperationOutcome::BLOCKLISTED);
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::BLOCKLISTED_WRITE);
    return;
  }

  auto copyable_callback = base::AdaptCallbackForRepeating(std::move(callback));
  query_result.characteristic->WriteRemoteCharacteristic(
      value,
      base::Bind(&WebBluetoothServiceImpl::OnCharacteristicWriteValueSuccess,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback),
      base::Bind(&WebBluetoothServiceImpl::OnCharacteristicWriteValueFailed,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback));
}
