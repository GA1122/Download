void SyncTest::TriggerSyncError(const browser_sync::SyncProtocolError& error,
                                SyncErrorFrequency frequency) {
  ASSERT_TRUE(ServerSupportsErrorTriggering());
  std::string path = "chromiumsync/error";
  int error_type =
      static_cast<int>(GetClientToServerResponseErrorType(
          error.error_type));
  int action = static_cast<int>(GetClientToServerResponseAction(
      error.action));

  path.append(base::StringPrintf("?error=%d", error_type));
  path.append(base::StringPrintf("&action=%d", action));

  path.append(base::StringPrintf("&error_description=%s",
                                 error.error_description.c_str()));
  path.append(base::StringPrintf("&url=%s", error.url.c_str()));
  path.append(base::StringPrintf("&frequency=%d", frequency));

  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
  std::string output = UTF16ToASCII(
      browser()->GetSelectedWebContents()->GetTitle());
   ASSERT_TRUE(output.find("SetError: 200") != string16::npos);
 }
