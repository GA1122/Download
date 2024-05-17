Status IndexedDBDatabase::GetAllOperation(
    int64_t object_store_id,
    int64_t index_id,
    std::unique_ptr<IndexedDBKeyRange> key_range,
    indexed_db::CursorType cursor_type,
    int64_t max_count,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1("IndexedDBDatabase::GetAllOperation", "txn.id", transaction->id());

  DCHECK_GT(max_count, 0);

  DCHECK(metadata_.object_stores.find(object_store_id) !=
         metadata_.object_stores.end());
  const IndexedDBObjectStoreMetadata& object_store_metadata =
      metadata_.object_stores[object_store_id];

  Status s = Status::OK();

  std::unique_ptr<IndexedDBBackingStore::Cursor> cursor;

  if (cursor_type == indexed_db::CURSOR_KEY_ONLY) {
    if (index_id == IndexedDBIndexMetadata::kInvalidId) {
      cursor = backing_store_->OpenObjectStoreKeyCursor(
          transaction->BackingStoreTransaction(), id(), object_store_id,
          *key_range, blink::mojom::IDBCursorDirection::Next, &s);
    } else {
      cursor = backing_store_->OpenIndexKeyCursor(
          transaction->BackingStoreTransaction(), id(), object_store_id,
          index_id, *key_range, blink::mojom::IDBCursorDirection::Next, &s);
    }
  } else {
    if (index_id == IndexedDBIndexMetadata::kInvalidId) {
      cursor = backing_store_->OpenObjectStoreCursor(
          transaction->BackingStoreTransaction(), id(), object_store_id,
          *key_range, blink::mojom::IDBCursorDirection::Next, &s);
    } else {
      cursor = backing_store_->OpenIndexCursor(
          transaction->BackingStoreTransaction(), id(), object_store_id,
          index_id, *key_range, blink::mojom::IDBCursorDirection::Next, &s);
    }
  }

  if (!s.ok()) {
    DLOG(ERROR) << "Unable to open cursor operation: " << s.ToString();
    return s;
  }

  std::vector<IndexedDBKey> found_keys;
  std::vector<IndexedDBReturnValue> found_values;
  if (!cursor) {
    callbacks->OnSuccessArray(&found_values);
    return s;
  }

  bool did_first_seek = false;
  bool generated_key = object_store_metadata.auto_increment &&
                       !object_store_metadata.key_path.IsNull();

  size_t response_size = blink::mojom::kIDBMaxMessageOverhead;
  int64_t num_found_items = 0;
  while (num_found_items++ < max_count) {
    bool cursor_valid;
    if (did_first_seek) {
      cursor_valid = cursor->Continue(&s);
    } else {
      cursor_valid = cursor->FirstSeek(&s);
      did_first_seek = true;
    }
    if (!s.ok())
      return s;

    if (!cursor_valid)
      break;

    IndexedDBReturnValue return_value;
    IndexedDBKey return_key;

    if (cursor_type == indexed_db::CURSOR_KEY_ONLY) {
      return_key = cursor->primary_key();
    } else {
      return_value.swap(*cursor->value());
      if (!return_value.empty() && generated_key) {
        return_value.primary_key = cursor->primary_key();
        return_value.key_path = object_store_metadata.key_path;
      }
    }

    if (cursor_type == indexed_db::CURSOR_KEY_ONLY)
      response_size += return_key.size_estimate();
    else
      response_size += return_value.SizeEstimate();
    if (response_size > GetUsableMessageSizeInBytes()) {
      callbacks->OnError(
          CreateError(blink::kWebIDBDatabaseExceptionUnknownError,
                      "Maximum IPC message size exceeded.", transaction));
      return s;
    }

    if (cursor_type == indexed_db::CURSOR_KEY_ONLY)
      found_keys.push_back(return_key);
    else
      found_values.push_back(return_value);
  }

  if (cursor_type == indexed_db::CURSOR_KEY_ONLY) {
    callbacks->OnSuccess(IndexedDBKey(std::move(found_keys)));
  } else {
    callbacks->OnSuccessArray(&found_values);
  }
  return s;
}
