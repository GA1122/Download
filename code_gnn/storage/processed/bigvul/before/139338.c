 void HostCache::AddEntry(const Key& key, const Entry& entry) {
  DCHECK_GT(max_entries_, size());
  DCHECK_EQ(0u, entries_.count(key));
  entries_.insert(std::make_pair(key, entry));
  DCHECK_GE(max_entries_, size());
}
