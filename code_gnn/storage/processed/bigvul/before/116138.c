bool ResourceDispatcherHostImpl::IsTransferredNavigation(
    const GlobalRequestID& transferred_request_id) const {
  return transferred_navigations_.find(transferred_request_id) !=
      transferred_navigations_.end();
}
