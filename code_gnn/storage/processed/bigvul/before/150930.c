void FakeCentral::SimulatePreconnectedPeripheral(
    const std::string& address,
    const std::string& name,
    const std::vector<device::BluetoothUUID>& known_service_uuids,
    SimulatePreconnectedPeripheralCallback callback) {
  FakePeripheral* fake_peripheral = GetFakePeripheral(address);
  if (fake_peripheral == nullptr) {
    auto fake_peripheral_ptr = std::make_unique<FakePeripheral>(this, address);
    fake_peripheral = fake_peripheral_ptr.get();
    auto pair = devices_.emplace(address, std::move(fake_peripheral_ptr));
    DCHECK(pair.second);
  }

  fake_peripheral->SetName(name);
  fake_peripheral->SetSystemConnected(true);
  fake_peripheral->SetServiceUUIDs(device::BluetoothDevice::UUIDSet(
      known_service_uuids.begin(), known_service_uuids.end()));

  std::move(callback).Run();
}
