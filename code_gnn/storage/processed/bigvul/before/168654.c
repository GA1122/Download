void IndexedDBTransaction::RemovePendingObservers(
    const std::vector<int32_t>& pending_observer_ids) {
  const auto& it = std::remove_if(
      pending_observers_.begin(), pending_observers_.end(),
      [&pending_observer_ids](const std::unique_ptr<IndexedDBObserver>& o) {
        return base::ContainsValue(pending_observer_ids, o->id());
      });
  if (it != pending_observers_.end())
    pending_observers_.erase(it, pending_observers_.end());
}
