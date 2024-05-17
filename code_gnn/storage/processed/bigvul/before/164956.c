void ResourceDispatcherHostImpl::ProcessBlockedRequestsForRoute(
    const GlobalFrameRoutingId& global_routing_id,
    bool cancel_requests) {
  auto iter = blocked_loaders_map_.find(global_routing_id);
  if (iter == blocked_loaders_map_.end()) {
    return;
  }

  BlockedLoadersList* loaders = iter->second.get();
  std::unique_ptr<BlockedLoadersList> deleter(std::move(iter->second));

  blocked_loaders_map_.erase(iter);

  for (std::unique_ptr<ResourceLoader>& loader : *loaders) {
    ResourceRequestInfoImpl* info = loader->GetRequestInfo();
    if (cancel_requests) {
      IncrementOutstandingRequestsMemory(-1, *info);
    } else {
      StartLoading(info, std::move(loader));
    }
  }
}
