void FakeCentral::AddFakeService(const std::string& peripheral_address,
                                 const device::BluetoothUUID& service_uuid,
                                 AddFakeServiceCallback callback) {
  FakePeripheral* fake_peripheral = GetFakePeripheral(peripheral_address);
  if (fake_peripheral == nullptr) {
    std::move(callback).Run(base::nullopt);
    return;
  }

  std::move(callback).Run(fake_peripheral->AddFakeService(service_uuid));
}
