bool LiveSyncTest::WaitForTestServerToStart(int time_ms, int intervals) {
  for (int i = 0; i < intervals; ++i) {
    if (IsTestServerRunning())
      return true;
    base::PlatformThread::Sleep(time_ms / intervals);
  }
  return false;
}
