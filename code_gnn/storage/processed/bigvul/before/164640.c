Status IndexedDBDatabase::OpenCursorOperation(
    std::unique_ptr<OpenCursorOperationParams> params,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1(
      "IndexedDBDatabase::OpenCursorOperation", "txn.id", transaction->id());

  if (params->task_type == blink::mojom::IDBTaskType::Preemptive)
    transaction->AddPreemptiveEvent();

  Status s = Status::OK();
  std::unique_ptr<IndexedDBBackingStore::Cursor> backing_store_cursor;
  if (params->index_id == IndexedDBIndexMetadata::kInvalidId) {
    if (params->cursor_type == indexed_db::CURSOR_KEY_ONLY) {
      DCHECK_EQ(params->task_type, blink::mojom::IDBTaskType::Normal);
      backing_store_cursor = backing_store_->OpenObjectStoreKeyCursor(
          transaction->BackingStoreTransaction(),
          id(),
          params->object_store_id,
          *params->key_range,
          params->direction,
          &s);
    } else {
      backing_store_cursor = backing_store_->OpenObjectStoreCursor(
          transaction->BackingStoreTransaction(),
          id(),
          params->object_store_id,
          *params->key_range,
          params->direction,
          &s);
    }
  } else {
    DCHECK_EQ(params->task_type, blink::mojom::IDBTaskType::Normal);
    if (params->cursor_type == indexed_db::CURSOR_KEY_ONLY) {
      backing_store_cursor = backing_store_->OpenIndexKeyCursor(
          transaction->BackingStoreTransaction(),
          id(),
          params->object_store_id,
          params->index_id,
          *params->key_range,
          params->direction,
          &s);
    } else {
      backing_store_cursor = backing_store_->OpenIndexCursor(
          transaction->BackingStoreTransaction(),
          id(),
          params->object_store_id,
          params->index_id,
          *params->key_range,
          params->direction,
          &s);
    }
  }

  if (!s.ok()) {
    DLOG(ERROR) << "Unable to open cursor operation: " << s.ToString();
    return s;
  }

  if (!backing_store_cursor) {
    params->callbacks->OnSuccess(nullptr);
    return s;
  }

  std::unique_ptr<IndexedDBCursor> cursor = std::make_unique<IndexedDBCursor>(
      std::move(backing_store_cursor), params->cursor_type, params->task_type,
      transaction);
  IndexedDBCursor* cursor_ptr = cursor.get();
  transaction->RegisterOpenCursor(cursor_ptr);
  params->callbacks->OnSuccess(std::move(cursor), cursor_ptr->key(),
                               cursor_ptr->primary_key(), cursor_ptr->Value());
  return s;
}
