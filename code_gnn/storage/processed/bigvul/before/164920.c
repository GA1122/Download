void ResourceDispatcherHostImpl::CancelRequestsForRoute(
    const GlobalFrameRoutingId& global_routing_id) {

  int child_id = global_routing_id.child_id;
  int route_id = global_routing_id.frame_routing_id;
  bool cancel_all_routes = (route_id == MSG_ROUTING_NONE);

  std::vector<GlobalRequestID> matching_requests;
  for (const auto& loader : pending_loaders_) {
    if (loader.first.child_id != child_id)
      continue;

    ResourceRequestInfoImpl* info = loader.second->GetRequestInfo();

    GlobalRequestID id(child_id, loader.first.request_id);
    DCHECK(id == loader.first);
    if (cancel_all_routes || route_id == info->GetRenderFrameID()) {
      if (info->keepalive() && !cancel_all_routes) {
      } else if (info->detachable_handler()) {
        info->detachable_handler()->Detach();
      } else if (!info->IsDownload() && !info->is_stream()) {
        matching_requests.push_back(id);
      }
    }
  }

  for (size_t i = 0; i < matching_requests.size(); ++i) {
    auto iter = pending_loaders_.find(matching_requests[i]);
    if (iter != pending_loaders_.end())
      RemovePendingLoader(iter);
  }

  if (!cancel_all_routes) {
    if (blocked_loaders_map_.find(global_routing_id) !=
        blocked_loaders_map_.end()) {
      CancelBlockedRequestsForRoute(global_routing_id);
    }
  } else {
    std::set<GlobalFrameRoutingId> routing_ids;
    for (const auto& blocked_loaders : blocked_loaders_map_) {
      if (blocked_loaders.first.child_id == child_id)
        routing_ids.insert(blocked_loaders.first);
    }
    for (const GlobalFrameRoutingId& frame_route_id : routing_ids) {
      CancelBlockedRequestsForRoute(frame_route_id);
    }
  }
}
