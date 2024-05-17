void CrosMock::InitMockPowerLibrary() {
  InitMockLibraryLoader();
  if (mock_power_library_)
    return;
  mock_power_library_ = new StrictMock<MockPowerLibrary>();
  test_api()->SetPowerLibrary(mock_power_library_, true);
}
