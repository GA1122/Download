Status IndexedDBDatabase::ClearOperation(
    int64_t object_store_id,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1("IndexedDBDatabase::ClearOperation", "txn.id", transaction->id());
  Status s = backing_store_->ClearObjectStore(
      transaction->BackingStoreTransaction(), id(), object_store_id);
  if (!s.ok())
    return s;
  callbacks->OnSuccess();

  FilterObservation(transaction, object_store_id,
                    blink::mojom::IDBOperationType::Clear, IndexedDBKeyRange(),
                    nullptr);
  factory_->NotifyIndexedDBContentChanged(
      origin(), metadata_.name, metadata_.object_stores[object_store_id].name);
  return s;
}
