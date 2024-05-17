FakeRemoteGattDescriptor* FakeCentral::GetFakeRemoteGattDescriptor(
    const std::string& peripheral_address,
    const std::string& service_id,
    const std::string& characteristic_id,
    const std::string& descriptor_id) const {
  FakeRemoteGattCharacteristic* fake_remote_gatt_characteristic =
      GetFakeRemoteGattCharacteristic(peripheral_address, service_id,
                                      characteristic_id);
  if (fake_remote_gatt_characteristic == nullptr) {
    return nullptr;
  }

  return static_cast<FakeRemoteGattDescriptor*>(
      fake_remote_gatt_characteristic->GetDescriptor(descriptor_id));
}
