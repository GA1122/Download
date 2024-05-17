void PlatformSensor::AddClient(Client* client) {
  DCHECK(client);
  clients_.AddObserver(client);
}
