void MemBackendImpl::OnEntryUpdated(MemEntryImpl* entry) {
  DCHECK(CheckLRUListOrder(lru_list_));
  entry->RemoveFromList();
  lru_list_.Append(entry);
}
