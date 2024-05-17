void FakeCentral::IsNotifying(const std::string& characteristic_id,
                              const std::string& service_id,
                              const std::string& peripheral_address,
                              IsNotifyingCallback callback) {
  FakeRemoteGattCharacteristic* fake_remote_gatt_characteristic =
      GetFakeRemoteGattCharacteristic(peripheral_address, service_id,
                                      characteristic_id);
  if (!fake_remote_gatt_characteristic) {
    std::move(callback).Run(false, false);
  }

  std::move(callback).Run(true, fake_remote_gatt_characteristic->IsNotifying());
}
