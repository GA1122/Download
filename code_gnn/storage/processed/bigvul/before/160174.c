int MemBackendImpl::CreateEntry(const std::string& key,
                                Entry** entry,
                                const CompletionCallback& callback) {
  std::pair<EntryMap::iterator, bool> create_result =
      entries_.insert(EntryMap::value_type(key, nullptr));
  const bool did_insert = create_result.second;
  if (!did_insert)
    return net::ERR_FAILED;

  MemEntryImpl* cache_entry =
      new MemEntryImpl(weak_factory_.GetWeakPtr(), key, net_log_);
  create_result.first->second = cache_entry;
  *entry = cache_entry;
  return net::OK;
}
