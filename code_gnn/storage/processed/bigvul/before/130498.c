bool QueryManager::HavePendingTransferQueries() {
  return !pending_transfer_queries_.empty();
}
