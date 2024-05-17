void ResourceDispatcherHostImpl::OnRenderFrameDeleted(
    const GlobalFrameRoutingId& global_routing_id) {
  CancelRequestsForRoute(global_routing_id);
}
