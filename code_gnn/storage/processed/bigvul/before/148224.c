leveldb::Status IndexedDBCursor::CursorPrefetchIterationOperation(
    int number_to_fetch,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction*  ) {
  IDB_TRACE("IndexedDBCursor::CursorPrefetchIterationOperation");
  leveldb::Status s = leveldb::Status::OK();

  std::vector<IndexedDBKey> found_keys;
  std::vector<IndexedDBKey> found_primary_keys;
  std::vector<IndexedDBValue> found_values;

  saved_cursor_.reset();
  const size_t max_size_estimate = 10 * 1024 * 1024;
  size_t size_estimate = 0;

  for (int i = 0; i < number_to_fetch; ++i) {
    if (!cursor_ || !cursor_->Continue(&s)) {
      cursor_.reset();
      if (s.ok()) {
        break;
      }
      Close();
      callbacks->OnError(
          IndexedDBDatabaseError(blink::kWebIDBDatabaseExceptionUnknownError,
                                 "Error continuing cursor."));
      return s;
    }

    if (i == 0) {
      saved_cursor_ = cursor_->Clone();
    }

    found_keys.push_back(cursor_->key());
    found_primary_keys.push_back(cursor_->primary_key());

    switch (cursor_type_) {
      case indexed_db::CURSOR_KEY_ONLY:
        found_values.push_back(IndexedDBValue());
        break;
      case indexed_db::CURSOR_KEY_AND_VALUE: {
        IndexedDBValue value;
        value.swap(*cursor_->value());
        size_estimate += value.SizeEstimate();
        found_values.push_back(value);
        break;
      }
      default:
        NOTREACHED();
    }
    size_estimate += cursor_->key().size_estimate();
    size_estimate += cursor_->primary_key().size_estimate();

    if (size_estimate > max_size_estimate)
      break;
  }

  if (found_keys.empty()) {
    callbacks->OnSuccess(nullptr);
    return s;
  }

  callbacks->OnSuccessWithPrefetch(
      found_keys, found_primary_keys, &found_values);
  return s;
}
