void SyncTest::TriggerBirthdayError() {
  ASSERT_TRUE(ServerSupportsErrorTriggering());
  std::string path = "chromiumsync/birthdayerror";
  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
  ASSERT_EQ("Birthday error",
            UTF16ToASCII(browser()->GetSelectedWebContents()->GetTitle()));
}
