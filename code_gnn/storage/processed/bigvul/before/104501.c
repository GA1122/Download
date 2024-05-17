void CrosMock::InitMockNetworkLibrary() {
  InitMockLibraryLoader();
  if (mock_network_library_)
    return;
  mock_network_library_ = new StrictMock<MockNetworkLibrary>();
  test_api()->SetNetworkLibrary(mock_network_library_, true);
}
