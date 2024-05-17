NOINLINE void ResetThread_CACHE() {
  volatile int inhibit_comdat = __LINE__;
  ALLOW_UNUSED_LOCAL(inhibit_comdat);
  BrowserThreadImpl::StopRedirectionOfThreadID(BrowserThread::CACHE);
}
