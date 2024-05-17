void DatabaseMessageFilter::RemoveObserver() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  db_tracker_->RemoveObserver(this);

  db_tracker_->CloseDatabases(database_connections_);
  database_connections_.RemoveAllConnections();
}
