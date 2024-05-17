void DownloadPathReservationTrackerTest::SetUp() {
  ASSERT_TRUE(test_download_dir_.CreateUniqueTempDir());
  set_default_download_path(test_download_dir_.path());
}
