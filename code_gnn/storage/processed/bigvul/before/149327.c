void IndexedDBConnection::AbortAllTransactions(
    const IndexedDBDatabaseError& error) {
  std::unordered_map<int64_t, std::unique_ptr<IndexedDBTransaction>> temp_map;
  std::swap(temp_map, transactions_);
  for (const auto& pair : temp_map) {
    IDB_TRACE1("IndexedDBDatabase::Abort(error)", "txn.id", pair.second->id());
    pair.second->Abort(error);
  }
}
