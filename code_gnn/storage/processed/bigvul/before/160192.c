void MemBackendImpl::OnExternalCacheHit(const std::string& key) {
  EntryMap::iterator it = entries_.find(key);
  if (it != entries_.end())
    it->second->UpdateStateOnUse(MemEntryImpl::ENTRY_WAS_NOT_MODIFIED);
}
