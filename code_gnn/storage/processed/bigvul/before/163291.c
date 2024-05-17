NOINLINE void ResetThread_PROCESS_LAUNCHER(
    std::unique_ptr<BrowserProcessSubThread> thread) {
  volatile int inhibit_comdat = __LINE__;
  ALLOW_UNUSED_LOCAL(inhibit_comdat);
  thread.reset();
}
