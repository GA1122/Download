CacheQueryResult WebBluetoothServiceImpl::QueryCacheForDevice(
    const blink::WebBluetoothDeviceId& device_id) {
  const std::string& device_address =
      allowed_devices().GetDeviceAddress(device_id);
  if (device_address.empty()) {
    CrashRendererAndClosePipe(bad_message::BDH_DEVICE_NOT_ALLOWED_FOR_ORIGIN);
    return CacheQueryResult(CacheQueryOutcome::BAD_RENDERER);
  }

  CacheQueryResult result;
  result.device = GetAdapter()->GetDevice(device_address);

  if (result.device == nullptr) {
    result.outcome = CacheQueryOutcome::NO_DEVICE;
  }
  return result;
}
