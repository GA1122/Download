void DatabaseImpl::IDBThreadHelper::OnGotUsageAndQuotaForCommit(
    int64_t transaction_id,
    storage::QuotaStatusCode status,
    int64_t usage,
    int64_t quota) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  if (status == storage::kQuotaStatusOk &&
      usage + transaction->size() <= quota) {
    connection_->database()->Commit(transaction);
  } else {
    connection_->AbortTransaction(
        transaction,
        IndexedDBDatabaseError(blink::kWebIDBDatabaseExceptionQuotaError));
  }
}
