void DatabaseImpl::IDBThreadHelper::AddObserver(int64_t transaction_id,
                                                int32_t observer_id,
                                                bool include_transaction,
                                                bool no_records,
                                                bool values,
                                                uint16_t operation_types) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  IndexedDBObserver::Options options(include_transaction, no_records, values,
                                     operation_types);
  connection_->database()->AddPendingObserver(transaction, observer_id,
                                              options);
}
