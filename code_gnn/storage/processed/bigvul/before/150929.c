void FakeCentral::SimulateGATTServicesChanged(
    const std::string& address,
    SimulateGATTServicesChangedCallback callback) {
  FakePeripheral* fake_peripheral = GetFakePeripheral(address);
  if (fake_peripheral == nullptr) {
    std::move(callback).Run(false);
    return;
  }

  std::move(callback).Run(true);
}
