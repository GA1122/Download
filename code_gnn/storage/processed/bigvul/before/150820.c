void WebBluetoothServiceImpl::RemoteDescriptorWriteValue(
    const std::string& descriptor_instance_id,
    const std::vector<uint8_t>& value,
    RemoteDescriptorWriteValueCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (value.size() > 512) {
    CrashRendererAndClosePipe(bad_message::BDH_INVALID_WRITE_VALUE_LENGTH);
    return;
  }

  const CacheQueryResult query_result =
      QueryCacheForDescriptor(descriptor_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordDescriptorWriteValueOutcome(query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult());
    return;
  }

  if (BluetoothBlocklist::Get().IsExcludedFromWrites(
          query_result.descriptor->GetUUID())) {
    RecordDescriptorWriteValueOutcome(UMAGATTOperationOutcome::BLOCKLISTED);
    std::move(callback).Run(
        blink::mojom::WebBluetoothResult::BLOCKLISTED_WRITE);
    return;
  }

  auto copyable_callback = base::AdaptCallbackForRepeating(std::move(callback));
  query_result.descriptor->WriteRemoteDescriptor(
      value,
      base::Bind(&WebBluetoothServiceImpl::OnDescriptorWriteValueSuccess,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback),
      base::Bind(&WebBluetoothServiceImpl::OnDescriptorWriteValueFailed,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback));
}
