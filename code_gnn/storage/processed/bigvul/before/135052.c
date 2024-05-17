bool AppCacheBackendImpl::UnregisterHost(int id) {
  HostMap::iterator found = hosts_.find(id);
  if (found == hosts_.end())
    return false;

  delete found->second;
  hosts_.erase(found);
  return true;
}
