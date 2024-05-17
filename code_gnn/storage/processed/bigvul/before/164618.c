void IndexedDBDatabase::DeleteIndex(IndexedDBTransaction* transaction,
                                    int64_t object_store_id,
                                    int64_t index_id) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::DeleteIndex", "txn.id", transaction->id());
  DCHECK_EQ(transaction->mode(),
            blink::mojom::IDBTransactionMode::VersionChange);

  if (!ValidateObjectStoreIdAndIndexId(object_store_id, index_id))
    return;

  transaction->ScheduleTask(
      base::BindOnce(&IndexedDBDatabase::DeleteIndexOperation, this,
                     object_store_id, index_id));
}
