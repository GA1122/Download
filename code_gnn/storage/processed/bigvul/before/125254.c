void DatabaseMessageFilter::OnDatabaseScheduledForDeletion(
    const string16& origin_identifier,
    const string16& database_name) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  Send(new DatabaseMsg_CloseImmediately(origin_identifier, database_name));
}
