void BuildCommitCommand::AddExtensionsActivityToMessage(
    SyncSession* session, CommitMessage* message) {
  ExtensionsActivityMonitor* monitor = session->context()->extensions_monitor();
  if (!session->status_controller()->HasBookmarkCommitActivity()) {
    monitor->PutRecords(session->extensions_activity());
    session->mutable_extensions_activity()->clear();
    return;
  }
  const ExtensionsActivityMonitor::Records& records =
      session->extensions_activity();
  for (ExtensionsActivityMonitor::Records::const_iterator it = records.begin();
       it != records.end(); ++it) {
    sync_pb::ChromiumExtensionsActivity* activity_message =
        message->add_extensions_activity();
    activity_message->set_extension_id(it->second.extension_id);
    activity_message->set_bookmark_writes_since_last_commit(
        it->second.bookmark_write_count);
  }
}
