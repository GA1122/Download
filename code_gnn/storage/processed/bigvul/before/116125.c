void ResourceDispatcherHostImpl::CancelRequestsForRoute(int child_id,
                                                        int route_id) {

  std::vector<GlobalRequestID> matching_requests;
  for (PendingRequestList::const_iterator i = pending_requests_.begin();
       i != pending_requests_.end(); ++i) {
    if (i->first.child_id == child_id) {
      ResourceRequestInfoImpl* info =
          ResourceRequestInfoImpl::ForRequest(i->second);
      GlobalRequestID id(child_id, i->first.request_id);
      DCHECK(id == i->first);
      if (!info->is_download() &&
          (transferred_navigations_.find(id) ==
               transferred_navigations_.end()) &&
          (route_id == -1 || route_id == info->GetRouteID())) {
        matching_requests.push_back(
            GlobalRequestID(child_id, i->first.request_id));
      }
    }
  }

  for (size_t i = 0; i < matching_requests.size(); ++i) {
    PendingRequestList::iterator iter =
        pending_requests_.find(matching_requests[i]);
    if (iter != pending_requests_.end())
      RemovePendingRequest(iter);
  }

  if (route_id != -1) {
    if (blocked_requests_map_.find(std::pair<int, int>(child_id, route_id)) !=
        blocked_requests_map_.end()) {
      CancelBlockedRequestsForRoute(child_id, route_id);
    }
  } else {
    std::set<int> route_ids;
    for (BlockedRequestMap::const_iterator iter = blocked_requests_map_.begin();
         iter != blocked_requests_map_.end(); ++iter) {
      if (iter->first.first == child_id)
        route_ids.insert(iter->first.second);
    }
    for (std::set<int>::const_iterator iter = route_ids.begin();
        iter != route_ids.end(); ++iter) {
      CancelBlockedRequestsForRoute(child_id, *iter);
    }
  }
}
