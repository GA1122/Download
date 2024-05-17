void IndexedDBDatabase::Get(IndexedDBTransaction* transaction,
                            int64_t object_store_id,
                            int64_t index_id,
                            std::unique_ptr<IndexedDBKeyRange> key_range,
                            bool key_only,
                            scoped_refptr<IndexedDBCallbacks> callbacks) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::Get", "txn.id", transaction->id());

  if (!ValidateObjectStoreIdAndOptionalIndexId(object_store_id, index_id))
    return;

  transaction->ScheduleTask(base::BindOnce(
      &IndexedDBDatabase::GetOperation, this, object_store_id, index_id,
      std::move(key_range),
      key_only ? indexed_db::CURSOR_KEY_ONLY : indexed_db::CURSOR_KEY_AND_VALUE,
      callbacks));
}
