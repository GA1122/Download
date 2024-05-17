void ResourceDispatcherHostImpl::OnRenderViewHostDeleted(int child_id,
                                                         int route_id) {
  auto* host = ResourceDispatcherHostImpl::Get();
  if (host && host->scheduler_) {
    host->scheduler_->OnClientDeleted(child_id, route_id);
  }
}
