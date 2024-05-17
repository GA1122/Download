void DatabaseMessageFilter::OnDatabaseSizeChanged(
    const string16& origin_identifier,
    const string16& database_name,
    int64 database_size) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  if (database_connections_.IsOriginUsed(origin_identifier)) {
    Send(new DatabaseMsg_UpdateSize(origin_identifier, database_name,
                                    database_size));
  }
}
