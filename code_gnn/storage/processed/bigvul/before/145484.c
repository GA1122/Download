void ResourceDispatcherHostImpl::CancelTransferringNavigation(
    const GlobalRequestID& id) {
  DCHECK(IsTransferredNavigation(id));
  RemovePendingRequest(id.child_id, id.request_id);
}
