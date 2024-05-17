void IndexedDBDatabase::DeleteObjectStore(IndexedDBTransaction* transaction,
                                          int64_t object_store_id) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::DeleteObjectStore", "txn.id",
             transaction->id());
  DCHECK_EQ(transaction->mode(), blink::kWebIDBTransactionModeVersionChange);

  if (!ValidateObjectStoreId(object_store_id))
    return;

  transaction->ScheduleTask(base::BindOnce(
      &IndexedDBDatabase::DeleteObjectStoreOperation, this, object_store_id));
}
