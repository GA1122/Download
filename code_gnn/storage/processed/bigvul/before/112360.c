void ResourceDispatcherHostImpl::ProcessBlockedRequestsForRoute(
    int child_id,
    int route_id,
    bool cancel_requests) {
  BlockedLoadersMap::iterator iter = blocked_loaders_map_.find(
      std::pair<int, int>(child_id, route_id));
  if (iter == blocked_loaders_map_.end()) {
    return;
  }

  BlockedLoadersList* loaders = iter->second;

  blocked_loaders_map_.erase(iter);

  for (BlockedLoadersList::iterator loaders_iter = loaders->begin();
       loaders_iter != loaders->end(); ++loaders_iter) {
    linked_ptr<ResourceLoader> loader = *loaders_iter;
    ResourceRequestInfoImpl* info = loader->GetRequestInfo();
    if (cancel_requests) {
      IncrementOutstandingRequestsMemoryCost(-1 * info->memory_cost(),
                                             info->GetChildID());
    } else {
      StartLoading(info, loader);
    }
  }

  delete loaders;
}
