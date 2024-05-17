void ResourceDispatcherHostImpl::BlockRequestsForRoute(int child_id,
                                                       int route_id) {
  std::pair<int, int> key(child_id, route_id);
  DCHECK(blocked_requests_map_.find(key) == blocked_requests_map_.end()) <<
      "BlockRequestsForRoute called  multiple time for the same RVH";
  blocked_requests_map_[key] = new BlockedRequestsList();
}
