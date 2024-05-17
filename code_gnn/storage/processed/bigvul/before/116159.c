void ResourceDispatcherHostImpl::ProcessBlockedRequestsForRoute(
    int child_id,
    int route_id,
    bool cancel_requests) {
  BlockedRequestMap::iterator iter = blocked_requests_map_.find(
      std::pair<int, int>(child_id, route_id));
  if (iter == blocked_requests_map_.end()) {
    return;
  }

  BlockedRequestsList* requests = iter->second;

  blocked_requests_map_.erase(iter);

  for (BlockedRequestsList::iterator req_iter = requests->begin();
       req_iter != requests->end(); ++req_iter) {
    net::URLRequest* request = *req_iter;
    ResourceRequestInfoImpl* info =
        ResourceRequestInfoImpl::ForRequest(request);
    IncrementOutstandingRequestsMemoryCost(-1 * info->memory_cost(),
                                           info->GetChildID());
    if (cancel_requests)
      delete request;
    else
      BeginRequestInternal(request);
  }

  delete requests;
}
