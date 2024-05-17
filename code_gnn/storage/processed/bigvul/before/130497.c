bool QueryManager::HavePendingQueries() {
  return !pending_queries_.empty();
}
