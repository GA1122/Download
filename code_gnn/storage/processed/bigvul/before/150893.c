bool FakeCentral::AllResponsesConsumed() {
  return std::all_of(devices_.begin(), devices_.end(), [](const auto& e) {
    FakePeripheral* fake_peripheral =
        static_cast<FakePeripheral*>(e.second.get());
    return fake_peripheral->AllResponsesConsumed();
  });
}
