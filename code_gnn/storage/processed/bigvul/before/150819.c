void WebBluetoothServiceImpl::RemoteDescriptorReadValue(
    const std::string& descriptor_instance_id,
    RemoteDescriptorReadValueCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  const CacheQueryResult query_result =
      QueryCacheForDescriptor(descriptor_instance_id);

  if (query_result.outcome == CacheQueryOutcome::BAD_RENDERER) {
    return;
  }

  if (query_result.outcome != CacheQueryOutcome::SUCCESS) {
    RecordDescriptorReadValueOutcome(query_result.outcome);
    std::move(callback).Run(query_result.GetWebResult(),
                            base::nullopt  );
    return;
  }

  if (BluetoothBlocklist::Get().IsExcludedFromReads(
          query_result.descriptor->GetUUID())) {
    RecordDescriptorReadValueOutcome(UMAGATTOperationOutcome::BLOCKLISTED);
    std::move(callback).Run(blink::mojom::WebBluetoothResult::BLOCKLISTED_READ,
                            base::nullopt  );
    return;
  }

  auto copyable_callback = base::AdaptCallbackForRepeating(std::move(callback));
  query_result.descriptor->ReadRemoteDescriptor(
      base::Bind(&WebBluetoothServiceImpl::OnDescriptorReadValueSuccess,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback),
      base::Bind(&WebBluetoothServiceImpl::OnDescriptorReadValueFailed,
                 weak_ptr_factory_.GetWeakPtr(), copyable_callback));
}
