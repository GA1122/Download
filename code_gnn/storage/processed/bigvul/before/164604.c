void IndexedDBDatabase::Close(IndexedDBConnection* connection, bool forced) {
  DCHECK(connections_.count(connection));
  DCHECK(connection->IsConnected());
  DCHECK(connection->database() == this);

  IDB_TRACE("IndexedDBDatabase::Close");

  connection->FinishAllTransactions(IndexedDBDatabaseError(
      blink::kWebIDBDatabaseExceptionUnknownError, "Connection is closing."));

  connections_.erase(connection);

  if (active_request_)
    active_request_->OnConnectionClosed(connection);

  if (connections_.empty() && !active_request_ && pending_requests_.empty()) {
    backing_store_ = nullptr;
    factory_->ReleaseDatabase(identifier_, forced);
  }
}
