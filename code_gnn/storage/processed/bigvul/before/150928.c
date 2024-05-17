void FakeCentral::SimulateGATTDisconnection(
    const std::string& address,
    SimulateGATTDisconnectionCallback callback) {
  FakePeripheral* fake_peripheral = GetFakePeripheral(address);
  if (fake_peripheral == nullptr) {
    std::move(callback).Run(false);
  }

  fake_peripheral->SimulateGATTDisconnection();
  std::move(callback).Run(true);
}
