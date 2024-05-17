void IndexedDBDatabase::GetAll(IndexedDBTransaction* transaction,
                               int64_t object_store_id,
                               int64_t index_id,
                               std::unique_ptr<IndexedDBKeyRange> key_range,
                               bool key_only,
                               int64_t max_count,
                               scoped_refptr<IndexedDBCallbacks> callbacks) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::GetAll", "txn.id", transaction->id());

  if (!ValidateObjectStoreId(object_store_id))
    return;

  transaction->ScheduleTask(base::BindOnce(
      &IndexedDBDatabase::GetAllOperation, this, object_store_id, index_id,
      std::move(key_range),
      key_only ? indexed_db::CURSOR_KEY_ONLY : indexed_db::CURSOR_KEY_AND_VALUE,
      max_count, callbacks));
}
