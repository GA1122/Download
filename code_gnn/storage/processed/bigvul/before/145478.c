void ResourceDispatcherHostImpl::BlockRequestsForRoute(int child_id,
                                                       int route_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  GlobalRoutingID key(child_id, route_id);
  DCHECK(blocked_loaders_map_.find(key) == blocked_loaders_map_.end()) <<
      "BlockRequestsForRoute called  multiple time for the same RVH";
  blocked_loaders_map_[key] = make_scoped_ptr(new BlockedLoadersList());
}
