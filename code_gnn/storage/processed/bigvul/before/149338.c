void IndexedDBConnection::RemoveObservers(
    const std::vector<int32_t>& observer_ids_to_remove) {
  std::vector<int32_t> pending_observer_ids;
  for (int32_t id_to_remove : observer_ids_to_remove) {
    const auto& it = std::find_if(
        active_observers_.begin(), active_observers_.end(),
        [&id_to_remove](const std::unique_ptr<IndexedDBObserver>& o) {
          return o->id() == id_to_remove;
        });
    if (it != active_observers_.end())
      active_observers_.erase(it);
    else
      pending_observer_ids.push_back(id_to_remove);
  }
  if (pending_observer_ids.empty())
    return;

  for (const auto& it : transactions_) {
    it.second->RemovePendingObservers(pending_observer_ids);
  }
}
