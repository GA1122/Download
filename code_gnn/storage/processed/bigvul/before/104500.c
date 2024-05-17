void CrosMock::InitMockLibraryLoader() {
  if (loader_)
    return;
  loader_ = new StrictMock<MockLibraryLoader>();
  EXPECT_CALL(*loader_, Load(_))
      .Times(AnyNumber())
      .WillRepeatedly(Return(true));
  test_api()->SetLibraryLoader(loader_, true);
}
