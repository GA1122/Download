void ResourceDispatcherHostImpl::RemovePendingLoader(
    const LoaderMap::iterator& iter) {
  ResourceRequestInfoImpl* info = iter->second->GetRequestInfo();

  if (info->keepalive())
    keepalive_statistics_recorder_.OnLoadFinished(info->GetChildID());

  IncrementOutstandingRequestsMemory(-1, *info);

  pending_loaders_.erase(iter);
}
