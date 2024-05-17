FakeRemoteGattService* FakeCentral::GetFakeRemoteGattService(
    const std::string& peripheral_address,
    const std::string& service_id) const {
  FakePeripheral* fake_peripheral = GetFakePeripheral(peripheral_address);
  if (fake_peripheral == nullptr) {
    return nullptr;
  }

  return static_cast<FakeRemoteGattService*>(
      fake_peripheral->GetGattService(service_id));
}
