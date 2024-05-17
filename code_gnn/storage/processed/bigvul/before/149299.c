void DatabaseImpl::IDBThreadHelper::DeleteIndex(int64_t transaction_id,
                                                int64_t object_store_id,
                                                int64_t index_id) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  connection_->database()->DeleteIndex(transaction, object_store_id, index_id);
}
