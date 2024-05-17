void DatabaseImpl::IDBThreadHelper::CreateIndex(
    int64_t transaction_id,
    int64_t object_store_id,
    int64_t index_id,
    const base::string16& name,
    const IndexedDBKeyPath& key_path,
    bool unique,
    bool multi_entry) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  connection_->database()->CreateIndex(transaction, object_store_id, index_id,
                                       name, key_path, unique, multi_entry);
}
