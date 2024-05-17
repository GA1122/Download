Status IndexedDBDatabase::GetKeyGeneratorCurrentNumberOperation(
    int64_t object_store_id,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    IndexedDBTransaction* transaction) {
  int64_t current_number;
  Status s = backing_store_.get()->GetKeyGeneratorCurrentNumber(
      transaction->BackingStoreTransaction(), id(), object_store_id,
      &current_number);
  if (!s.ok()) {
    callbacks->OnError(CreateError(
        blink::kWebIDBDatabaseExceptionDataError,
        "Failed to get the current number of key generator.", transaction));
    return s;
  }
  callbacks->OnSuccess(current_number);
  return s;
}
