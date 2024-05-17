std::unique_ptr<SendTabToSelfEntry> SendTabToSelfEntry::FromLocalProto(
    const SendTabToSelfLocal& local_entry,
    base::Time now) {
  std::unique_ptr<SendTabToSelfEntry> to_return =
      FromProto(local_entry.specifics(), now);
  if (to_return) {
    to_return->SetNotificationDismissed(local_entry.notification_dismissed());
  }
  return to_return;
}
