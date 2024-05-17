void DownloadPathReservationTrackerTest::TestReservedPathCallback(
    base::FilePath* return_path, bool* return_verified, bool* did_run_callback,
    const base::FilePath& path, bool verified) {
  *did_run_callback = true;
  *return_path = path;
  *return_verified = verified;
}
