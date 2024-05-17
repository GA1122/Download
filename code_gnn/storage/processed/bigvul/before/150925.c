void FakeCentral::SetNextWriteDescriptorResponse(
    uint16_t gatt_code,
    const std::string& descriptor_id,
    const std::string& characteristic_id,
    const std::string& service_id,
    const std::string& peripheral_address,
    SetNextWriteDescriptorResponseCallback callback) {
  FakeRemoteGattDescriptor* fake_remote_gatt_descriptor =
      GetFakeRemoteGattDescriptor(peripheral_address, service_id,
                                  characteristic_id, descriptor_id);
  if (!fake_remote_gatt_descriptor) {
    std::move(callback).Run(false);
  }

  fake_remote_gatt_descriptor->SetNextWriteResponse(gatt_code);
  std::move(callback).Run(true);
}
