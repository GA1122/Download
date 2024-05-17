Status IndexedDBDatabase::DeleteIndexOperation(
    int64_t object_store_id,
    int64_t index_id,
    IndexedDBTransaction* transaction) {
  IDB_TRACE1(
      "IndexedDBDatabase::DeleteIndexOperation", "txn.id", transaction->id());

  IndexedDBIndexMetadata index_metadata =
      RemoveIndex(object_store_id, index_id);

  Status s = metadata_coding_->DeleteIndex(
      transaction->BackingStoreTransaction()->transaction(),
      transaction->database()->id(), object_store_id, index_metadata);

  if (!s.ok())
    return s;

  s = backing_store_->ClearIndex(transaction->BackingStoreTransaction(),
                                 transaction->database()->id(), object_store_id,
                                 index_id);
  if (!s.ok()) {
    AddIndex(object_store_id, std::move(index_metadata),
             IndexedDBIndexMetadata::kInvalidId);
    return s;
  }

  transaction->ScheduleAbortTask(
      base::BindOnce(&IndexedDBDatabase::DeleteIndexAbortOperation, this,
                     object_store_id, std::move(index_metadata)));
  return s;
}
