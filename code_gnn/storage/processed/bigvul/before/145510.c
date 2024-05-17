void ResourceDispatcherHostImpl::OnCancelRequest(int request_id) {
  int child_id = filter_->child_id();

  if (IsTransferredNavigation(GlobalRequestID(child_id, request_id)))
    return;

  ResourceLoader* loader = GetLoader(child_id, request_id);
  if (!loader) {
    DVLOG(1) << "Canceling a request that wasn't found";
    return;
  }

  loader->CancelRequest(true);
}
