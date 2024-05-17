void ResourceDispatcherHostImpl::RemovePendingRequest(int child_id,
                                                      int request_id) {
  LoaderMap::iterator i = pending_loaders_.find(
      GlobalRequestID(child_id, request_id));
  if (i == pending_loaders_.end()) {
    NOTREACHED() << "Trying to remove a request that's not here";
    return;
  }
  RemovePendingLoader(i);
}
