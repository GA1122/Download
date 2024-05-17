void IndexedDBTransaction::AddObservation(
    int32_t connection_id,
    ::indexed_db::mojom::ObservationPtr observation) {
  auto it = connection_changes_map_.find(connection_id);
  if (it == connection_changes_map_.end()) {
    it = connection_changes_map_
             .insert(std::make_pair(
                 connection_id, ::indexed_db::mojom::ObserverChanges::New()))
             .first;
  }
  it->second->observations.push_back(std::move(observation));
}
