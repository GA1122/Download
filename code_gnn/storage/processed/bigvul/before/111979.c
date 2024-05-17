void SyncTest::CleanUpOnMainThread() {
  browser::CloseAllBrowsers();
  ui_test_utils::RunAllPendingInMessageLoop();

  CHECK_EQ(0U, BrowserList::size());
  clients_.reset();
}
