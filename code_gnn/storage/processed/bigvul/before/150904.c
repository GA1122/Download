void FakeCentral::GetLastWrittenDescriptorValue(
    const std::string& descriptor_id,
    const std::string& characteristic_id,
    const std::string& service_id,
    const std::string& peripheral_address,
    GetLastWrittenDescriptorValueCallback callback) {
  FakeRemoteGattDescriptor* fake_remote_gatt_descriptor =
      GetFakeRemoteGattDescriptor(peripheral_address, service_id,
                                  characteristic_id, descriptor_id);
  if (!fake_remote_gatt_descriptor) {
    std::move(callback).Run(false, base::nullopt);
  }

  std::move(callback).Run(true,
                          fake_remote_gatt_descriptor->last_written_value());
}
