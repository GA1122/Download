HostCache::Entry* HostCache::LookupInternal(const Key& key) {
  auto it = entries_.find(key);
  return (it != entries_.end()) ? &it->second : nullptr;
}
