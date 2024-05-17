CacheQueryResult WebBluetoothServiceImpl::QueryCacheForService(
    const std::string& service_instance_id) {
  auto device_iter = service_id_to_device_address_.find(service_instance_id);

  if (device_iter == service_id_to_device_address_.end()) {
    CrashRendererAndClosePipe(bad_message::BDH_INVALID_SERVICE_ID);
    return CacheQueryResult(CacheQueryOutcome::BAD_RENDERER);
  }

  const blink::WebBluetoothDeviceId* device_id =
      allowed_devices().GetDeviceId(device_iter->second);
  if (device_id == nullptr) {
    CrashRendererAndClosePipe(bad_message::BDH_DEVICE_NOT_ALLOWED_FOR_ORIGIN);
    return CacheQueryResult(CacheQueryOutcome::BAD_RENDERER);
  }

  CacheQueryResult result = QueryCacheForDevice(*device_id);
  if (result.outcome != CacheQueryOutcome::SUCCESS) {
    return result;
  }

  result.service = result.device->GetGattService(service_instance_id);
  if (result.service == nullptr) {
    result.outcome = CacheQueryOutcome::NO_SERVICE;
  } else if (!allowed_devices().IsAllowedToAccessService(
                 *device_id, result.service->GetUUID())) {
    CrashRendererAndClosePipe(bad_message::BDH_SERVICE_NOT_ALLOWED_FOR_ORIGIN);
    return CacheQueryResult(CacheQueryOutcome::BAD_RENDERER);
  }
  return result;
}
