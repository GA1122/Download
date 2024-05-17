 leveldb::Status IndexedDBCursor::CursorAdvanceOperation(
     uint32_t count,
     scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction*  ) {
  IDB_TRACE("IndexedDBCursor::CursorAdvanceOperation");
  leveldb::Status s = leveldb::Status::OK();

  if (!cursor_ || !cursor_->Advance(count, &s)) {
    cursor_.reset();
    if (s.ok()) {
      callbacks->OnSuccess(nullptr);
      return s;
    }
    Close();
    callbacks->OnError(IndexedDBDatabaseError(
        blink::kWebIDBDatabaseExceptionUnknownError, "Error advancing cursor"));
    return s;
  }

  callbacks->OnSuccess(key(), primary_key(), Value());
  return s;
}
