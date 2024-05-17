NOINLINE void ResetThread_PROCESS_LAUNCHER() {
  volatile int inhibit_comdat = __LINE__;
  ALLOW_UNUSED_LOCAL(inhibit_comdat);
  BrowserThreadImpl::StopRedirectionOfThreadID(BrowserThread::PROCESS_LAUNCHER);
}
