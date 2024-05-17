bool SyncTest::WaitForTestServerToStart(int time_ms, int intervals) {
  for (int i = 0; i < intervals; ++i) {
    if (IsTestServerRunning())
      return true;
    base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(
        time_ms / intervals));
  }
  return false;
}
