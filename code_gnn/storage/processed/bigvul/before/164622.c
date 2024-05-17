Status IndexedDBDatabase::DeleteObjectStoreOperation(
    int64_t object_store_id,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1("IndexedDBDatabase::DeleteObjectStoreOperation",
             "txn.id",
             transaction->id());

  IndexedDBObjectStoreMetadata object_store_metadata =
      RemoveObjectStore(object_store_id);

  Status s = metadata_coding_->DeleteObjectStore(
      transaction->BackingStoreTransaction()->transaction(),
      transaction->database()->id(), object_store_metadata);

  if (!s.ok()) {
    AddObjectStore(std::move(object_store_metadata),
                   IndexedDBObjectStoreMetadata::kInvalidId);
    return s;
  }

  s = backing_store_->ClearObjectStore(transaction->BackingStoreTransaction(),
                                       transaction->database()->id(),
                                       object_store_id);

  if (!s.ok()) {
    AddObjectStore(std::move(object_store_metadata),
                   IndexedDBObjectStoreMetadata::kInvalidId);
    return s;
  }
  transaction->ScheduleAbortTask(
      base::BindOnce(&IndexedDBDatabase::DeleteObjectStoreAbortOperation, this,
                     std::move(object_store_metadata)));
  return s;
}
