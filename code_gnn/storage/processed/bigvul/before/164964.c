void ResourceDispatcherHostImpl::ResumeBlockedRequestsForRoute(
    const GlobalFrameRoutingId& global_routing_id) {
  ProcessBlockedRequestsForRoute(global_routing_id, false);
}
