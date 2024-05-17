scoped_ptr<AppCacheHost> AppCacheBackendImpl::TransferHostOut(int host_id) {
  HostMap::iterator found = hosts_.find(host_id);
  if (found == hosts_.end()) {
    NOTREACHED();
    return scoped_ptr<AppCacheHost>();
  }

  AppCacheHost* transferree = found->second;

  found->second = new AppCacheHost(host_id, frontend_, service_);

  transferree->PrepareForTransfer();
  return scoped_ptr<AppCacheHost>(transferree);
}
