void CrosMock::InitMockTouchpadLibrary() {
  InitMockLibraryLoader();
  if (mock_touchpad_library_)
    return;
  mock_touchpad_library_ = new StrictMock<MockTouchpadLibrary>();
  test_api()->SetTouchpadLibrary(mock_touchpad_library_, true);
}
