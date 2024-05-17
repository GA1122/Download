FakeDownloadItem* DownloadPathReservationTrackerTest::CreateDownloadItem(
    int32 id) {
  FakeDownloadItem* item = new ::testing::StrictMock<FakeDownloadItem>;
  DownloadId download_id(reinterpret_cast<void*>(this), id);
  EXPECT_CALL(*item, GetGlobalId())
      .WillRepeatedly(Return(download_id));
  EXPECT_CALL(*item, GetTargetFilePath())
      .WillRepeatedly(ReturnRefOfCopy(base::FilePath()));
  return item;
}
