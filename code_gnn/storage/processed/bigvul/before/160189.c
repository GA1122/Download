void MemBackendImpl::OnEntryDoomed(MemEntryImpl* entry) {
  DCHECK(CheckLRUListOrder(lru_list_));
  if (entry->type() == MemEntryImpl::PARENT_ENTRY)
    entries_.erase(entry->key());
  entry->RemoveFromList();
}
