void ResourceDispatcherHostImpl::CancelRequestsForRoute(int child_id,
                                                        int route_id) {

  std::vector<GlobalRequestID> matching_requests;
  for (LoaderMap::const_iterator i = pending_loaders_.begin();
       i != pending_loaders_.end(); ++i) {
    if (i->first.child_id != child_id)
      continue;

    ResourceRequestInfoImpl* info = i->second->GetRequestInfo();

    GlobalRequestID id(child_id, i->first.request_id);
    DCHECK(id == i->first);

    if (!info->is_download() && !IsTransferredNavigation(id) &&
        (route_id == -1 || route_id == info->GetRouteID())) {
      matching_requests.push_back(id);
    }
  }

  for (size_t i = 0; i < matching_requests.size(); ++i) {
    LoaderMap::iterator iter = pending_loaders_.find(matching_requests[i]);
    if (iter != pending_loaders_.end())
      RemovePendingLoader(iter);
  }

  if (route_id != -1) {
    if (blocked_loaders_map_.find(ProcessRouteIDs(child_id, route_id)) !=
        blocked_loaders_map_.end()) {
      CancelBlockedRequestsForRoute(child_id, route_id);
    }
  } else {
    std::set<int> route_ids;
    for (BlockedLoadersMap::const_iterator iter = blocked_loaders_map_.begin();
         iter != blocked_loaders_map_.end(); ++iter) {
      if (iter->first.first == child_id)
        route_ids.insert(iter->first.second);
    }
    for (std::set<int>::const_iterator iter = route_ids.begin();
        iter != route_ids.end(); ++iter) {
      CancelBlockedRequestsForRoute(child_id, *iter);
    }
  }
}
