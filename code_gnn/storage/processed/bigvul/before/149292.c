void DatabaseImpl::IDBThreadHelper::Count(
    int64_t transaction_id,
    int64_t object_store_id,
    int64_t index_id,
    const IndexedDBKeyRange& key_range,
    scoped_refptr<IndexedDBCallbacks> callbacks) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  connection_->database()->Count(transaction, object_store_id, index_id,
                                 base::MakeUnique<IndexedDBKeyRange>(key_range),
                                 std::move(callbacks));
}
