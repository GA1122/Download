void ResourceDispatcherHostImpl::OnRenderViewHostSetIsLoading(int child_id,
                                                              int route_id,
                                                              bool is_loading) {
  scheduler_->OnLoadingStateChanged(child_id, route_id, !is_loading);
}
