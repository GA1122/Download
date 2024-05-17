leveldb::Status IndexedDBCursor::CursorIterationOperation(
    std::unique_ptr<IndexedDBKey> key,
    std::unique_ptr<IndexedDBKey> primary_key,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction*  ) {
  IDB_TRACE("IndexedDBCursor::CursorIterationOperation");
  leveldb::Status s = leveldb::Status::OK();

  if (!cursor_ ||
      !cursor_->Continue(key.get(), primary_key.get(),
                         IndexedDBBackingStore::Cursor::SEEK, &s)) {
    cursor_.reset();
    if (s.ok()) {
      callbacks->OnSuccess(nullptr);
      return s;
    }
    Close();
    callbacks->OnError(
        IndexedDBDatabaseError(blink::kWebIDBDatabaseExceptionUnknownError,
                               "Error continuing cursor."));
    return s;
  }

  callbacks->OnSuccess(this->key(), this->primary_key(), Value());
  return s;
}
