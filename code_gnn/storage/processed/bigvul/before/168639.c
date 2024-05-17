void IndexedDBTransaction::AddPendingObserver(
    int32_t observer_id,
    const IndexedDBObserver::Options& options) {
  DCHECK_NE(mode(), blink::kWebIDBTransactionModeVersionChange);
  pending_observers_.push_back(std::make_unique<IndexedDBObserver>(
      observer_id, object_store_ids_, options));
}
