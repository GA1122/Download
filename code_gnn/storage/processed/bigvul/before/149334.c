IndexedDBTransaction* IndexedDBConnection::GetTransaction(int64_t id) const {
  auto it = transactions_.find(id);
  if (it == transactions_.end())
    return nullptr;
  return it->second.get();
}
