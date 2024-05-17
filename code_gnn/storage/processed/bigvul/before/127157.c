void DownloadPathReservationTrackerTest::CallGetReservedPath(
    DownloadItem& download_item,
    const base::FilePath& target_path,
    bool uniquify_path,
    base::FilePath* return_path,
    bool* return_verified) {
  base::WeakPtrFactory<DownloadPathReservationTrackerTest> weak_ptr_factory(
      this);
  bool did_run_callback = false;
  DownloadPathReservationTracker::GetReservedPath(
      download_item, target_path, default_download_path(), uniquify_path,
      base::Bind(&DownloadPathReservationTrackerTest::TestReservedPathCallback,
                 weak_ptr_factory.GetWeakPtr(), return_path, return_verified,
                 &did_run_callback));
  message_loop_.RunUntilIdle();
  EXPECT_TRUE(did_run_callback);
}
