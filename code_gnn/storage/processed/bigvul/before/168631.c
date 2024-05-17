void IndexedDBDatabase::SendObservations(
    std::map<int32_t, ::indexed_db::mojom::ObserverChangesPtr> changes_map) {
  for (auto* conn : connections_) {
    auto it = changes_map.find(conn->id());
    if (it == changes_map.end())
      continue;

    ::indexed_db::mojom::ObserverChangesPtr& changes = it->second;
    for (const auto& transaction_pair : changes->transaction_map) {
      std::set<int64_t> scope(transaction_pair.second->scope.begin(),
                              transaction_pair.second->scope.end());
      IndexedDBTransaction* transaction = conn->CreateTransaction(
          transaction_pair.second->id, scope,
          blink::kWebIDBTransactionModeReadOnly,
          new IndexedDBBackingStore::Transaction(backing_store_.get()));
      DCHECK(transaction);
      transaction_coordinator_.DidCreateObserverTransaction(transaction);
      transaction_count_++;
      transaction->GrabSnapshotThenStart();
    }

    conn->callbacks()->OnDatabaseChange(std::move(it->second));
  }
}
