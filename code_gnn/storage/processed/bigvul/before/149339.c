void IndexedDBConnection::RemoveTransaction(int64_t id) {
  transactions_.erase(id);
}
