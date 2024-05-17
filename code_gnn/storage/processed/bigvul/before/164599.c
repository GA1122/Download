void IndexedDBDatabase::AddPendingObserver(
    IndexedDBTransaction* transaction,
    int32_t observer_id,
    const IndexedDBObserver::Options& options) {
  DCHECK(transaction);
  transaction->AddPendingObserver(observer_id, options);
}
