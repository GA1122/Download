void IndexedDBDatabase::RenameIndex(IndexedDBTransaction* transaction,
                                    int64_t object_store_id,
                                    int64_t index_id,
                                    const base::string16& new_name) {
  DCHECK(transaction);
  IDB_TRACE1("IndexedDBDatabase::RenameIndex", "txn.id", transaction->id());
  DCHECK_EQ(transaction->mode(),
            blink::mojom::IDBTransactionMode::VersionChange);

  if (!ValidateObjectStoreIdAndIndexId(object_store_id, index_id))
    return;


  IndexedDBIndexMetadata& index_metadata =
      metadata_.object_stores[object_store_id].indexes[index_id];

  base::string16 old_name;
  Status s = metadata_coding_->RenameIndex(
      transaction->BackingStoreTransaction()->transaction(),
      transaction->database()->id(), object_store_id, new_name, &old_name,
      &index_metadata);

  if (!s.ok()) {
    ReportErrorWithDetails(s, "Internal error renaming index.");
    return;
  }
  DCHECK_EQ(index_metadata.name, new_name);

  transaction->ScheduleAbortTask(
      base::BindOnce(&IndexedDBDatabase::RenameIndexAbortOperation, this,
                     object_store_id, index_id, std::move(old_name)));
}
