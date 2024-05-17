void ResourceDispatcherHostImpl::CancelBlockedRequestsForRoute(
    const GlobalFrameRoutingId& global_routing_id) {
  ProcessBlockedRequestsForRoute(global_routing_id, true);
}
