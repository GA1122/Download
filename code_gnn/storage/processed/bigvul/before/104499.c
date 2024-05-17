void CrosMock::InitMockCryptohomeLibrary() {
  InitMockLibraryLoader();
  if (mock_cryptohome_library_)
    return;
  mock_cryptohome_library_ = new StrictMock<MockCryptohomeLibrary>();
  test_api()->SetCryptohomeLibrary(mock_cryptohome_library_, true);
}
