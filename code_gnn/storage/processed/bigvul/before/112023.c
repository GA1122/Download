void SyncTest::TriggerTransientError() {
  ASSERT_TRUE(ServerSupportsErrorTriggering());
  std::string path = "chromiumsync/transienterror";
  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
  ASSERT_EQ("Transient error",
            UTF16ToASCII(browser()->GetSelectedWebContents()->GetTitle()));
}
