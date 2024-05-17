NOINLINE void ResetThread_FILE_USER_BLOCKING() {
  volatile int inhibit_comdat = __LINE__;
  ALLOW_UNUSED_LOCAL(inhibit_comdat);
  BrowserThreadImpl::StopRedirectionOfThreadID(
      BrowserThread::FILE_USER_BLOCKING);
}
