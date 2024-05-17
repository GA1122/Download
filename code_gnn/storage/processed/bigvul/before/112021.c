void SyncTest::TriggerNotification(
    syncable::ModelTypeSet changed_types) {
  ASSERT_TRUE(ServerSupportsNotificationControl());
  const std::string& data =
      sync_notifier::P2PNotificationData("from_server",
                                         sync_notifier::NOTIFY_ALL,
                                         changed_types).ToString();
  const std::string& path =
      std::string("chromiumsync/sendnotification?channel=") +
      sync_notifier::kSyncP2PNotificationChannel + "&data=" + data;
  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
  ASSERT_EQ("Notification sent",
            UTF16ToASCII(browser()->GetSelectedWebContents()->GetTitle()));
}
