void ResourceDispatcherHost::CancelRequestsForRoute(int child_id,
                                                    int route_id) {

  std::vector<GlobalRequestID> matching_requests;
  for (PendingRequestList::const_iterator i = pending_requests_.begin();
       i != pending_requests_.end(); ++i) {
    if (i->first.child_id == child_id) {
      ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);
      if (!info->is_download() &&
          (route_id == -1 || route_id == info->route_id())) {
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
