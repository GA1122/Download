void InProcessBrowserTest::TearDown() {
  DCHECK(!g_browser_process);
#if defined(OS_WIN)
  com_initializer_.reset();
#endif
  BrowserTestBase::TearDown();
}
