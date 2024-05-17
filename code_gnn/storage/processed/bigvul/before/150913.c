void FakeCentral::RemoveFakeService(const std::string& identifier,
                                    const std::string& peripheral_address,
                                    RemoveFakeServiceCallback callback) {
  FakePeripheral* fake_peripheral = GetFakePeripheral(peripheral_address);
  if (!fake_peripheral) {
    std::move(callback).Run(false);
    return;
  }
  std::move(callback).Run(fake_peripheral->RemoveFakeService(identifier));
}
