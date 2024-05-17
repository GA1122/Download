int MemBackendImpl::CalculateSizeOfEntriesBetween(
    base::Time initial_time,
    base::Time end_time,
    const CompletionCallback& callback) {
  if (end_time.is_null())
    end_time = Time::Max();
  DCHECK_GE(end_time, initial_time);

  int size = 0;
  base::LinkNode<MemEntryImpl>* node = lru_list_.head();
  while (node != lru_list_.end() && node->value()->GetLastUsed() < initial_time)
    node = node->next();
  while (node != lru_list_.end() && node->value()->GetLastUsed() < end_time) {
    MemEntryImpl* entry = node->value();
    size += entry->GetStorageSize();
    node = node->next();
  }
  return size;
}
