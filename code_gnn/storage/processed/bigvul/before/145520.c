void ResourceDispatcherHostImpl::ProcessBlockedRequestsForRoute(
    int child_id,
    int route_id,
    bool cancel_requests) {
  BlockedLoadersMap::iterator iter = blocked_loaders_map_.find(
      GlobalRoutingID(child_id, route_id));
  if (iter == blocked_loaders_map_.end()) {
    return;
  }

  BlockedLoadersList* loaders = iter->second.get();
  scoped_ptr<BlockedLoadersList> deleter(std::move(iter->second));

  blocked_loaders_map_.erase(iter);

  for (scoped_ptr<ResourceLoader>& loader : *loaders) {
    ResourceRequestInfoImpl* info = loader->GetRequestInfo();
    if (cancel_requests) {
      IncrementOutstandingRequestsMemory(-1, *info);
    } else {
      StartLoading(info, std::move(loader));
    }
  }
}
