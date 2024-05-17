void FakeCentral::GetLastWrittenCharacteristicValue(
    const std::string& characteristic_id,
    const std::string& service_id,
    const std::string& peripheral_address,
    GetLastWrittenCharacteristicValueCallback callback) {
  FakeRemoteGattCharacteristic* fake_remote_gatt_characteristic =
      GetFakeRemoteGattCharacteristic(peripheral_address, service_id,
                                      characteristic_id);
  if (fake_remote_gatt_characteristic == nullptr) {
    std::move(callback).Run(false, base::nullopt);
  }

  std::move(callback).Run(
      true, fake_remote_gatt_characteristic->last_written_value());
}
