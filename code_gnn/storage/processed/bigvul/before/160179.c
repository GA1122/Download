int MemBackendImpl::DoomEntry(const std::string& key,
                              const CompletionCallback& callback) {
  EntryMap::iterator it = entries_.find(key);
  if (it == entries_.end())
    return net::ERR_FAILED;

  it->second->Doom();
  return net::OK;
}
