void CrosMock::InitMockInputMethodLibrary() {
  InitMockLibraryLoader();
  if (mock_input_method_library_)
    return;
  mock_input_method_library_ = new StrictMock<MockInputMethodLibrary>();
  test_api()->SetInputMethodLibrary(mock_input_method_library_, true);
}
