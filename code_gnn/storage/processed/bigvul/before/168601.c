void IndexedDBDatabase::AbortAllTransactionsForConnections() {
  IDB_TRACE("IndexedDBDatabase::AbortAllTransactionsForConnections");

  for (IndexedDBConnection* connection : connections_) {
    connection->AbortAllTransactions(
        IndexedDBDatabaseError(blink::kWebIDBDatabaseExceptionUnknownError,
                               "Database is compacting."));
  }
}
