void IndexedDBDatabase::CreateIndex(IndexedDBTransaction* transaction,
                                    int64_t object_store_id,
                                    int64_t index_id,
                                    const base::string16& name,
                                    const IndexedDBKeyPath& key_path,
                                    bool unique,
                                    bool multi_entry) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::CreateIndex", "txn.id", transaction->id());
  DCHECK_EQ(transaction->mode(),
            blink::mojom::IDBTransactionMode::VersionChange);

  if (!ValidateObjectStoreIdAndNewIndexId(object_store_id, index_id))
    return;

  UMA_HISTOGRAM_ENUMERATION("WebCore.IndexedDB.Schema.Index.KeyPathType",
                            HistogramKeyPathType(key_path), KEY_PATH_TYPE_MAX);
  UMA_HISTOGRAM_BOOLEAN("WebCore.IndexedDB.Schema.Index.Unique", unique);
  UMA_HISTOGRAM_BOOLEAN("WebCore.IndexedDB.Schema.Index.MultiEntry",
                        multi_entry);

  IndexedDBIndexMetadata index_metadata;

  Status s = metadata_coding_->CreateIndex(
      transaction->BackingStoreTransaction()->transaction(),
      transaction->database()->id(), object_store_id, index_id, name, key_path,
      unique, multi_entry, &index_metadata);

  if (!s.ok()) {
    base::string16 error_string =
        ASCIIToUTF16("Internal error creating index '") + index_metadata.name +
        ASCIIToUTF16("'.");
    transaction->Abort(IndexedDBDatabaseError(
        blink::kWebIDBDatabaseExceptionUnknownError, error_string));
    return;
  }

  AddIndex(object_store_id, std::move(index_metadata), index_id);
  transaction->ScheduleAbortTask(
      base::BindOnce(&IndexedDBDatabase::CreateIndexAbortOperation, this,
                     object_store_id, index_id));
}
