Status IndexedDBDatabase::SetIndexesReadyOperation(
    size_t index_count,
    IndexedDBTransaction* transaction) {
  for (size_t i = 0; i < index_count; ++i)
    transaction->DidCompletePreemptiveEvent();
  return Status::OK();
}
