FakeRemoteGattCharacteristic* FakeCentral::GetFakeRemoteGattCharacteristic(
    const std::string& peripheral_address,
    const std::string& service_id,
    const std::string& characteristic_id) const {
  FakeRemoteGattService* fake_remote_gatt_service =
      GetFakeRemoteGattService(peripheral_address, service_id);
  if (fake_remote_gatt_service == nullptr) {
    return nullptr;
  }

  return static_cast<FakeRemoteGattCharacteristic*>(
      fake_remote_gatt_service->GetCharacteristic(characteristic_id));
}
