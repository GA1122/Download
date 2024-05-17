void ResourceDispatcherHostImpl::ResumeBlockedRequestsForRoute(int child_id,
                                                               int route_id) {
  ProcessBlockedRequestsForRoute(child_id, route_id, false);
}
