 void FakeCentral::SetNextGATTConnectionResponse(
     const std::string& address,
     uint16_t code,
    SetNextGATTConnectionResponseCallback callback) {
  FakePeripheral* fake_peripheral = GetFakePeripheral(address);
  if (fake_peripheral == nullptr) {
    std::move(callback).Run(false);
    return;
  }

  fake_peripheral->SetNextGATTConnectionResponse(code);
  std::move(callback).Run(true);
}
