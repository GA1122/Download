void AppCacheBackendImpl::TransferHostIn(
    int new_host_id, scoped_ptr<AppCacheHost> host) {
  HostMap::iterator found = hosts_.find(new_host_id);
  if (found == hosts_.end()) {
    NOTREACHED();
    return;
  }

  delete found->second;

  host->CompleteTransfer(new_host_id, frontend_);
  found->second = host.release();
}
