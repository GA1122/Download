void IndexedDBDatabase::Count(IndexedDBTransaction* transaction,
                              int64_t object_store_id,
                              int64_t index_id,
                              std::unique_ptr<IndexedDBKeyRange> key_range,
                              scoped_refptr<IndexedDBCallbacks> callbacks) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::Count", "txn.id", transaction->id());

  if (!ValidateObjectStoreIdAndOptionalIndexId(object_store_id, index_id))
    return;

  transaction->ScheduleTask(base::BindOnce(&IndexedDBDatabase::CountOperation,
                                           this, object_store_id, index_id,
                                           std::move(key_range), callbacks));
}
