void CrosMock::InitMockScreenLockLibrary() {
  InitMockLibraryLoader();
  if (mock_screen_lock_library_)
    return;
  mock_screen_lock_library_ = new StrictMock<MockScreenLockLibrary>();
  test_api()->SetScreenLockLibrary(mock_screen_lock_library_, true);
}
