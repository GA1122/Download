bool PlatformSensor::StopListening(Client* client) {
  DCHECK(client);
  auto client_entry = config_map_.find(client);
  if (client_entry == config_map_.end())
    return false;

  config_map_.erase(client_entry);

  return UpdateSensorInternal(config_map_);
}
