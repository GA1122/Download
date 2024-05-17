  void WaitUntilBackgrounded(const base::Process& lhs,
                             bool lhs_backgrounded,
                             const base::Process& rhs,
                             bool rhs_backgrounded) {
    while (IsProcessBackgrounded(lhs) != lhs_backgrounded ||
           IsProcessBackgrounded(rhs) != rhs_backgrounded) {
      base::RunLoop().RunUntilIdle();
      base::PlatformThread::Sleep(TestTimeouts::tiny_timeout());
    }
  }
