void SyncTest::TriggerAuthError() {
  ASSERT_TRUE(ServerSupportsErrorTriggering());
  std::string path = "chromiumsync/cred";
  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
}
