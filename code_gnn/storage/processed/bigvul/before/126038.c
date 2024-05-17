IPC::Message* TabAppendedNotificationObserver::ReleaseReply() {
  return reply_message_.release();
}
