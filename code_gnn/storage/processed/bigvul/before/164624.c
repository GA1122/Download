Status IndexedDBDatabase::DeleteRangeOperation(
    int64_t object_store_id,
    std::unique_ptr<IndexedDBKeyRange> key_range,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1("IndexedDBDatabase::DeleteRangeOperation", "txn.id",
             transaction->id());
  Status s = backing_store_->DeleteRange(transaction->BackingStoreTransaction(),
                                         id(), object_store_id, *key_range);
  if (!s.ok())
    return s;
  callbacks->OnSuccess();
  FilterObservation(transaction, object_store_id,
                    blink::mojom::IDBOperationType::Delete, *key_range,
                    nullptr);
  factory_->NotifyIndexedDBContentChanged(
      origin(), metadata_.name, metadata_.object_stores[object_store_id].name);
  return s;
}
