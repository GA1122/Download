void FakeCentral::RemoveFakeCharacteristic(
    const std::string& identifier,
    const std::string& service_id,
    const std::string& peripheral_address,
    RemoveFakeCharacteristicCallback callback) {
  FakeRemoteGattService* fake_remote_gatt_service =
      GetFakeRemoteGattService(peripheral_address, service_id);
  if (fake_remote_gatt_service == nullptr) {
    std::move(callback).Run(false);
    return;
  }

  std::move(callback).Run(
      fake_remote_gatt_service->RemoveFakeCharacteristic(identifier));
}
