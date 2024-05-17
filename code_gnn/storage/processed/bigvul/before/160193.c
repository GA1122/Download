int MemBackendImpl::OpenEntry(const std::string& key,
                              Entry** entry,
                              const CompletionCallback& callback) {
  EntryMap::iterator it = entries_.find(key);
  if (it == entries_.end())
    return net::ERR_FAILED;

  it->second->Open();

  *entry = it->second;
  return net::OK;
}
