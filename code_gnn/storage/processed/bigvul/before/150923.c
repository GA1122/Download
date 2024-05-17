void FakeCentral::SetNextSubscribeToNotificationsResponse(
    uint16_t gatt_code,
    const std::string& characteristic_id,
    const std::string& service_id,
    const std::string& peripheral_address,
    SetNextSubscribeToNotificationsResponseCallback callback) {
  FakeRemoteGattCharacteristic* fake_remote_gatt_characteristic =
      GetFakeRemoteGattCharacteristic(peripheral_address, service_id,
                                      characteristic_id);
  if (fake_remote_gatt_characteristic == nullptr) {
    std::move(callback).Run(false);
  }

  fake_remote_gatt_characteristic->SetNextSubscribeToNotificationsResponse(
      gatt_code);
  std::move(callback).Run(true);
}
