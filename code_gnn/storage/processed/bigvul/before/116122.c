void ResourceDispatcherHostImpl::CancelRequest(int child_id,
                                               int request_id,
                                               bool from_renderer) {
  GlobalRequestID id(child_id, request_id);
  if (from_renderer) {
    if (transferred_navigations_.find(id) != transferred_navigations_.end())
      return;
  }

  PendingRequestList::iterator i = pending_requests_.find(id);
  if (i == pending_requests_.end()) {
    DVLOG(1) << "Canceling a request that wasn't found";
    return;
  }
  net::URLRequest* request = i->second;

  bool started_before_cancel = request->is_pending();
  if (CancelRequestInternal(request, from_renderer) &&
      !started_before_cancel) {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&ResourceDispatcherHostImpl::CallResponseCompleted,
                   base::Unretained(this),
                   child_id,
                   request_id));
  }
}
