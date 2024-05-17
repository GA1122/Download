void IndexedDBDatabase::SetIndexKeys(
    IndexedDBTransaction* transaction,
    int64_t object_store_id,
    std::unique_ptr<IndexedDBKey> primary_key,
    const std::vector<IndexedDBIndexKeys>& index_keys) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::SetIndexKeys", "txn.id", transaction->id());
  DCHECK_EQ(transaction->mode(),
            blink::mojom::IDBTransactionMode::VersionChange);

  IndexedDBBackingStore::RecordIdentifier record_identifier;
  bool found = false;
  Status s = backing_store_->KeyExistsInObjectStore(
      transaction->BackingStoreTransaction(), metadata_.id, object_store_id,
      *primary_key, &record_identifier, &found);
  if (!s.ok()) {
    ReportErrorWithDetails(s, "Internal error setting index keys.");
    return;
  }
  if (!found) {
    transaction->Abort(IndexedDBDatabaseError(
        blink::kWebIDBDatabaseExceptionUnknownError,
        "Internal error setting index keys for object store."));
    return;
  }

  std::vector<std::unique_ptr<IndexWriter>> index_writers;
  base::string16 error_message;
  bool obeys_constraints = false;
  DCHECK(metadata_.object_stores.find(object_store_id) !=
         metadata_.object_stores.end());
  const IndexedDBObjectStoreMetadata& object_store_metadata =
      metadata_.object_stores[object_store_id];
  bool backing_store_success = MakeIndexWriters(transaction,
                                                backing_store_.get(),
                                                id(),
                                                object_store_metadata,
                                                *primary_key,
                                                false,
                                                index_keys,
                                                &index_writers,
                                                &error_message,
                                                &obeys_constraints);
  if (!backing_store_success) {
    transaction->Abort(IndexedDBDatabaseError(
        blink::kWebIDBDatabaseExceptionUnknownError,
        "Internal error: backing store error updating index keys."));
    return;
  }
  if (!obeys_constraints) {
    transaction->Abort(IndexedDBDatabaseError(
        blink::kWebIDBDatabaseExceptionConstraintError, error_message));
    return;
  }

  for (const auto& writer : index_writers) {
    writer->WriteIndexKeys(record_identifier, backing_store_.get(),
                           transaction->BackingStoreTransaction(), id(),
                           object_store_id);
  }
}
