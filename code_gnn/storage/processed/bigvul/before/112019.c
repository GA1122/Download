 void SyncTest::TriggerCreateSyncedBookmarks() {
   ASSERT_TRUE(ServerSupportsErrorTriggering());
   std::string path = "chromiumsync/createsyncedbookmarks";
  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
  ASSERT_EQ("Synced Bookmarks",
            UTF16ToASCII(browser()->GetSelectedWebContents()->GetTitle()));
}
