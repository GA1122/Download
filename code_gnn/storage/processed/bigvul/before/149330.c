void IndexedDBConnection::ActivatePendingObservers(
    std::vector<std::unique_ptr<IndexedDBObserver>> pending_observers) {
  for (auto& observer : pending_observers) {
    active_observers_.push_back(std::move(observer));
  }
  pending_observers.clear();
}
