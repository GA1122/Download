void FakeCentral::AddFakeDescriptor(
    const device::BluetoothUUID& descriptor_uuid,
    const std::string& characteristic_id,
    const std::string& service_id,
    const std::string& peripheral_address,
    AddFakeDescriptorCallback callback) {
  FakeRemoteGattCharacteristic* fake_remote_gatt_characteristic =
      GetFakeRemoteGattCharacteristic(peripheral_address, service_id,
                                      characteristic_id);
  if (fake_remote_gatt_characteristic == nullptr) {
    std::move(callback).Run(base::nullopt);
  }

  std::move(callback).Run(
      fake_remote_gatt_characteristic->AddFakeDescriptor(descriptor_uuid));
}
