void IndexedDBDatabase::SendObservations(
    std::map<int32_t, blink::mojom::IDBObserverChangesPtr> changes_map) {
  for (auto* conn : connections_) {
    auto it = changes_map.find(conn->id());
    if (it != changes_map.end())
      conn->callbacks()->OnDatabaseChange(std::move(it->second));
  }
}
