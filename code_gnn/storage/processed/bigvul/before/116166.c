void ResourceDispatcherHostImpl::StartDeferredRequest(int child_id,
                                                      int request_id) {
  GlobalRequestID global_id(child_id, request_id);
  PendingRequestList::iterator i = pending_requests_.find(global_id);
  if (i == pending_requests_.end()) {
    LOG(WARNING) << "Trying to resume a non-existent request ("
                 << child_id << ", " << request_id << ")";
    return;
  }


  StartRequest(i->second);
}
