PlatformSensorProviderBase::GetPendingRequestTypes() {
  std::vector<mojom::SensorType> request_types;
  for (auto const& entry : requests_map_)
    request_types.push_back(entry.first);
   return request_types;
 }
