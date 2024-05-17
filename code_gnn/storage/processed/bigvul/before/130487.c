void QueryManager::Destroy(bool have_context) {
  pending_queries_.clear();
  pending_transfer_queries_.clear();
  while (!queries_.empty()) {
    Query* query = queries_.begin()->second.get();
    query->Destroy(have_context);
    queries_.erase(queries_.begin());
  }
}
