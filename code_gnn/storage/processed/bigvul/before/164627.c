void IndexedDBDatabase::FilterObservation(IndexedDBTransaction* transaction,
                                          int64_t object_store_id,
                                          blink::mojom::IDBOperationType type,
                                          const IndexedDBKeyRange& key_range,
                                          const IndexedDBValue* value) {
  for (auto* connection : connections_) {
    bool recorded = false;
    for (const auto& observer : connection->active_observers()) {
      if (!observer->IsRecordingType(type) ||
          !observer->IsRecordingObjectStore(object_store_id))
        continue;
      if (!recorded) {
        auto observation = blink::mojom::IDBObservation::New();
        observation->object_store_id = object_store_id;
        observation->type = type;
        if (type != blink::mojom::IDBOperationType::Clear)
          observation->key_range = key_range;
        transaction->AddObservation(connection->id(), std::move(observation));
        recorded = true;
      }
      blink::mojom::IDBObserverChangesPtr& changes =
          *transaction->GetPendingChangesForConnection(connection->id());

      changes->observation_index_map[observer->id()].push_back(
          changes->observations.size() - 1);
      if (value && observer->values() && !changes->observations.back()->value) {
        IndexedDBValue copy = *value;
        changes->observations.back()->value =
            IndexedDBValue::ConvertAndEraseValue(&copy);
      }
    }
  }
}
