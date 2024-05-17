FakePeripheral* FakeCentral::GetFakePeripheral(
    const std::string& peripheral_address) const {
  auto device_iter = devices_.find(peripheral_address);
  if (device_iter == devices_.end()) {
    return nullptr;
  }

  return static_cast<FakePeripheral*>(device_iter->second.get());
}
