void CrosMock::SetPowerLibraryExpectations() {
  EXPECT_CALL(*mock_power_library_, EnableScreenLock(_))
      .Times(AnyNumber());
}
