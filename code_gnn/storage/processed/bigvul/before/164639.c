void IndexedDBDatabase::OpenCursor(
    IndexedDBTransaction* transaction,
    int64_t object_store_id,
    int64_t index_id,
    std::unique_ptr<IndexedDBKeyRange> key_range,
    blink::mojom::IDBCursorDirection direction,
    bool key_only,
    blink::mojom::IDBTaskType task_type,
    scoped_refptr<IndexedDBCallbacks> callbacks) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::OpenCursor", "txn.id", transaction->id());

  if (!ValidateObjectStoreIdAndOptionalIndexId(object_store_id, index_id))
    return;

  std::unique_ptr<OpenCursorOperationParams> params(
      std::make_unique<OpenCursorOperationParams>());
  params->object_store_id = object_store_id;
  params->index_id = index_id;
  params->key_range = std::move(key_range);
  params->direction = direction;
  params->cursor_type =
      key_only ? indexed_db::CURSOR_KEY_ONLY : indexed_db::CURSOR_KEY_AND_VALUE;
  params->task_type = task_type;
  params->callbacks = callbacks;
  transaction->ScheduleTask(base::BindOnce(
      &IndexedDBDatabase::OpenCursorOperation, this, std::move(params)));
}
