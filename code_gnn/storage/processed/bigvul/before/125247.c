void DatabaseMessageFilter::OnDatabaseClosed(const string16& origin_identifier,
                                             const string16& database_name) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  if (!database_connections_.IsDatabaseOpened(
          origin_identifier, database_name)) {
    RecordAction(UserMetricsAction("BadMessageTerminate_DBMF"));
    BadMessageReceived();
    return;
  }

  database_connections_.RemoveConnection(origin_identifier, database_name);
  db_tracker_->DatabaseClosed(origin_identifier, database_name);
}
