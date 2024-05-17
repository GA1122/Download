bool DownloadPathReservationTrackerTest::IsPathInUse(
    const base::FilePath& path) {
  return DownloadPathReservationTracker::IsPathInUseForTesting(path);
}
