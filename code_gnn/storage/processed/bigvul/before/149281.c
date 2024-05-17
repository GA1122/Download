void DatabaseImpl::IDBThreadHelper::AbortWithError(
    int64_t transaction_id,
    scoped_refptr<IndexedDBCallbacks> callbacks,
    const IndexedDBDatabaseError& error) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());

  callbacks->OnError(error);

  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  connection_->AbortTransaction(transaction, error);
}
