bool PlatformSensor::StopListening(Client* client,
                                   const PlatformSensorConfiguration& config) {
  DCHECK(clients_.HasObserver(client));
  auto client_entry = config_map_.find(client);
  if (client_entry == config_map_.end())
    return false;

  auto& config_list = client_entry->second;
  auto config_entry = std::find(config_list.begin(), config_list.end(), config);
  if (config_entry == config_list.end())
    return false;

  config_list.erase(config_entry);

  return UpdateSensorInternal(config_map_);
}
