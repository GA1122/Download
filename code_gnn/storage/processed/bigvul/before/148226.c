leveldb::Status InvokeOrSucceed(base::WeakPtr<IndexedDBCursor> weak_cursor,
                                IndexedDBTransaction::Operation operation,
                                IndexedDBTransaction* transaction) {
  if (weak_cursor)
    return operation.Run(transaction);
  return leveldb::Status::OK();
}
