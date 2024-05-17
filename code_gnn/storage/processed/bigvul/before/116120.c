void ResourceDispatcherHostImpl::CallResponseCompleted(int child_id,
                                                       int request_id) {
  PendingRequestList::iterator i = pending_requests_.find(
      GlobalRequestID(child_id, request_id));
  if (i != pending_requests_.end())
    ResponseCompleted(i->second);
}
