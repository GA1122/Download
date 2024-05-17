void PageSnapshotTaker::Observe(int type,
                                const content::NotificationSource& source,
                                const content::NotificationDetails& details) {
  SendMessage(false, "a modal dialog is active");
}
