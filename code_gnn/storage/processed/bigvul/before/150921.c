void FakeCentral::SetNextReadCharacteristicResponse(
    uint16_t gatt_code,
    const base::Optional<std::vector<uint8_t>>& value,
    const std::string& characteristic_id,
    const std::string& service_id,
    const std::string& peripheral_address,
    SetNextReadCharacteristicResponseCallback callback) {
  FakeRemoteGattCharacteristic* fake_remote_gatt_characteristic =
      GetFakeRemoteGattCharacteristic(peripheral_address, service_id,
                                      characteristic_id);
  if (fake_remote_gatt_characteristic == nullptr) {
    std::move(callback).Run(false);
  }

  fake_remote_gatt_characteristic->SetNextReadResponse(gatt_code, value);
  std::move(callback).Run(true);
}
