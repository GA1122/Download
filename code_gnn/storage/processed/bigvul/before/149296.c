void DatabaseImpl::IDBThreadHelper::CreateObjectStore(
    int64_t transaction_id,
    int64_t object_store_id,
    const base::string16& name,
    const IndexedDBKeyPath& key_path,
    bool auto_increment) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  connection_->database()->CreateObjectStore(transaction, object_store_id, name,
                                             key_path, auto_increment);
}
