void InProcessBrowserTest::TearDown() {
  DCHECK(!g_browser_process);
  BrowserTestBase::TearDown();
}
