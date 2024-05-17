void IndexedDBDatabase::Put(
    IndexedDBTransaction* transaction,
    int64_t object_store_id,
    IndexedDBValue* value,
    std::vector<std::unique_ptr<storage::BlobDataHandle>>* handles,
    std::unique_ptr<IndexedDBKey> key,
    blink::WebIDBPutMode put_mode,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    const std::vector<IndexedDBIndexKeys>& index_keys) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::Put", "txn.id", transaction->id());
  DCHECK_NE(transaction->mode(), blink::kWebIDBTransactionModeReadOnly);

  if (!ValidateObjectStoreId(object_store_id))
    return;

  DCHECK(key);
  DCHECK(value);
  std::unique_ptr<PutOperationParams> params(
      std::make_unique<PutOperationParams>());
  params->object_store_id = object_store_id;
  params->value.swap(*value);
  params->handles.swap(*handles);
  params->key = std::move(key);
  params->put_mode = put_mode;
  params->callbacks = callbacks;
  params->index_keys = index_keys;
  transaction->ScheduleTask(base::BindOnce(&IndexedDBDatabase::PutOperation,
                                           this, std::move(params)));
}