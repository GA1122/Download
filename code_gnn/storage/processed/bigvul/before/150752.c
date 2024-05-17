std::unique_ptr<device::BluetoothDiscoveryFilter> ComputeScanFilter(
    const base::Optional<
        std::vector<blink::mojom::WebBluetoothLeScanFilterPtr>>& filters) {
  std::unordered_set<BluetoothUUID, device::BluetoothUUIDHash> services;

  if (filters) {
    for (const auto& filter : filters.value()) {
      if (!filter->services) {
        continue;
      }
      for (const auto& service : filter->services.value()) {
        services.insert(service);
      }
    }
  }

  auto discovery_filter = std::make_unique<device::BluetoothDiscoveryFilter>(
      device::BLUETOOTH_TRANSPORT_LE);
  for (const BluetoothUUID& service : services) {
    discovery_filter->AddUUID(service);
  }
  return discovery_filter;
}
