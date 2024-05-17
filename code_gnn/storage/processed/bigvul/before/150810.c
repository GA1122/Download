CacheQueryResult WebBluetoothServiceImpl::QueryCacheForCharacteristic(
    const std::string& characteristic_instance_id) {
  auto characteristic_iter =
      characteristic_id_to_service_id_.find(characteristic_instance_id);

  if (characteristic_iter == characteristic_id_to_service_id_.end()) {
    CrashRendererAndClosePipe(bad_message::BDH_INVALID_CHARACTERISTIC_ID);
    return CacheQueryResult(CacheQueryOutcome::BAD_RENDERER);
  }

  CacheQueryResult result = QueryCacheForService(characteristic_iter->second);

  if (result.outcome != CacheQueryOutcome::SUCCESS) {
    return result;
  }

  result.characteristic =
      result.service->GetCharacteristic(characteristic_instance_id);

  if (result.characteristic == nullptr) {
    result.outcome = CacheQueryOutcome::NO_CHARACTERISTIC;
  }

  return result;
}
