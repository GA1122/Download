bool ResourceDispatcherHostImpl::IsValidRequest(net::URLRequest* request) {
  if (!request)
    return false;
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  return pending_requests_.find(
      GlobalRequestID(info->GetChildID(), info->GetRequestID())) !=
      pending_requests_.end();
}
