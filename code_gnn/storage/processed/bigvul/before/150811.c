CacheQueryResult WebBluetoothServiceImpl::QueryCacheForDescriptor(
    const std::string& descriptor_instance_id) {
  auto descriptor_iter =
      descriptor_id_to_characteristic_id_.find(descriptor_instance_id);

  if (descriptor_iter == descriptor_id_to_characteristic_id_.end()) {
    CrashRendererAndClosePipe(bad_message::BDH_INVALID_DESCRIPTOR_ID);
    return CacheQueryResult(CacheQueryOutcome::BAD_RENDERER);
  }

  CacheQueryResult result =
      QueryCacheForCharacteristic(descriptor_iter->second);

  if (result.outcome != CacheQueryOutcome::SUCCESS) {
    return result;
  }

  result.descriptor =
      result.characteristic->GetDescriptor(descriptor_instance_id);

  if (result.descriptor == nullptr) {
    result.outcome = CacheQueryOutcome::NO_DESCRIPTOR;
  }

  return result;
}
