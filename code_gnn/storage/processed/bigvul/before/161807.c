void PlatformSensorProviderLinux::ProcessStoredRequests() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  std::vector<mojom::SensorType> request_types = GetPendingRequestTypes();
  if (request_types.empty())
    return;

  for (auto const& type : request_types) {
    if (IsFusionSensorType(type))
      continue;

    SensorInfoLinux* device = nullptr;
    auto device_entry = sensor_devices_by_type_.find(type);
    if (device_entry != sensor_devices_by_type_.end())
      device = device_entry->second.get();
    CreateSensorAndNotify(type, device);
  }
}
