void WebBluetoothServiceImpl::DeviceAdvertisementReceived(
    const std::string& device_address,
    const base::Optional<std::string>& device_name,
    const base::Optional<std::string>& advertisement_name,
    base::Optional<int8_t> rssi,
    base::Optional<int8_t> tx_power,
    base::Optional<uint16_t> appearance,
    const device::BluetoothDevice::UUIDList& advertised_uuids,
    const device::BluetoothDevice::ServiceDataMap& service_data_map,
    const device::BluetoothDevice::ManufacturerDataMap& manufacturer_data_map) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!discovery_session_ || !discovery_session_->IsActive())
    return;

  auto client = scanning_clients_.begin();
  while (client != scanning_clients_.end()) {
    auto device = blink::mojom::WebBluetoothDevice::New();
    device->id = allowed_devices().AddDevice(device_address);
    device->name = device_name;

    auto result = blink::mojom::WebBluetoothScanResult::New();
    result->device = std::move(device);

    result->name = advertisement_name;

    result->appearance_is_set = appearance.has_value();
    result->appearance = appearance.value_or( 0xffc0);

    result->rssi_is_set = rssi.has_value();
    result->rssi = rssi.value_or( 128);

    result->tx_power_is_set = tx_power.has_value();
    result->tx_power = tx_power.value_or( 128);

    std::vector<device::BluetoothUUID> uuids;
    for (auto& uuid : advertised_uuids)
      uuids.push_back(device::BluetoothUUID(uuid.canonical_value()));
    result->uuids = std::move(uuids);

    auto& manufacturer_data = result->manufacturer_data;
    for (auto& it : manufacturer_data_map)
      manufacturer_data.emplace(it.first, it.second);

    base::flat_map<std::string, std::vector<uint8_t>> services;
    for (auto& it : service_data_map)
      services[it.first.canonical_value()] = it.second;
    result->service_data = std::move(services);

    bool okay = (*client)->SendEvent(std::move(result));
    if (!okay) {
      client = scanning_clients_.erase(client);
      continue;
    }

    ++client;
  }

  if (scanning_clients_.empty()) {
    discovery_session_->Stop(base::DoNothing(), base::DoNothing());
    discovery_session_ = nullptr;
    return;
  }
}
