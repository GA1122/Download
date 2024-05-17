void CrosMock::SetTouchpadLibraryExpectations() {
  EXPECT_CALL(*mock_touchpad_library_, SetSensitivity(_))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_touchpad_library_, SetTapToClick(_))
      .Times(AnyNumber());
}
