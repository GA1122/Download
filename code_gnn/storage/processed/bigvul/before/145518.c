void ResourceDispatcherHostImpl::OnShutdown() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  is_shutdown_ = true;
  pending_loaders_.clear();

  update_load_states_timer_.reset();

  std::set<GlobalRoutingID> ids;
  for (const auto& blocked_loaders : blocked_loaders_map_) {
    std::pair<std::set<GlobalRoutingID>::iterator, bool> result =
        ids.insert(blocked_loaders.first);
    DCHECK(result.second);
  }
  for (const auto& routing_id : ids) {
    CancelBlockedRequestsForRoute(routing_id.child_id, routing_id.route_id);
  }

  scheduler_.reset();
}
