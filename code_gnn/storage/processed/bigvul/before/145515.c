void ResourceDispatcherHostImpl::OnRenderViewHostDeleted(
    int child_id,
    int route_id) {
  scheduler_->OnClientDeleted(child_id, route_id);
  CancelRequestsForRoute(child_id, route_id);
}
