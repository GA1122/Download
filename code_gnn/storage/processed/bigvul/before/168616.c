void IndexedDBDatabase::FilterObservation(IndexedDBTransaction* transaction,
                                          int64_t object_store_id,
                                          blink::WebIDBOperationType type,
                                          const IndexedDBKeyRange& key_range,
                                          const IndexedDBValue* value) {
  for (auto* connection : connections_) {
    bool recorded = false;
    for (const auto& observer : connection->active_observers()) {
      if (!observer->IsRecordingType(type) ||
          !observer->IsRecordingObjectStore(object_store_id))
        continue;
      if (!recorded) {
        auto observation = ::indexed_db::mojom::Observation::New();
        observation->object_store_id = object_store_id;
        observation->type = type;
        if (type != blink::kWebIDBClear)
          observation->key_range = key_range;
        transaction->AddObservation(connection->id(), std::move(observation));
        recorded = true;
      }
      ::indexed_db::mojom::ObserverChangesPtr& changes =
          *transaction->GetPendingChangesForConnection(connection->id());

      changes->observation_index_map[observer->id()].push_back(
          changes->observations.size() - 1);
      if (observer->include_transaction() &&
          !base::ContainsKey(changes->transaction_map, observer->id())) {
        auto mojo_transaction = ::indexed_db::mojom::ObserverTransaction::New();
        mojo_transaction->id = connection->NewObserverTransactionId();
        mojo_transaction->scope.insert(mojo_transaction->scope.end(),
                                       observer->object_store_ids().begin(),
                                       observer->object_store_ids().end());
        changes->transaction_map[observer->id()] = std::move(mojo_transaction);
      }
      if (value && observer->values() && !changes->observations.back()->value) {
        IndexedDBValue copy = *value;
        changes->observations.back()->value =
            IndexedDBCallbacks::ConvertAndEraseValue(&copy);
      }
    }
  }
}
