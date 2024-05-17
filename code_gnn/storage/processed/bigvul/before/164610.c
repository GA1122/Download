std::unique_ptr<IndexedDBConnection> IndexedDBDatabase::CreateConnection(
    scoped_refptr<IndexedDBDatabaseCallbacks> database_callbacks,
    int child_process_id) {
  std::unique_ptr<IndexedDBConnection> connection(
      std::make_unique<IndexedDBConnection>(child_process_id, this,
                                            database_callbacks));
  connections_.insert(connection.get());
  backing_store_->GrantChildProcessPermissions(child_process_id);
  return connection;
}
