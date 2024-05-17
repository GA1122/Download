bool ResourceDispatcherHostImpl::CompleteRead(net::URLRequest* request,
                                              int* bytes_read) {
  if (!request || !request->status().is_success()) {
    NOTREACHED();
    return false;
  }

  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  bool defer = false;
  if (!info->resource_handler()->OnReadCompleted(info->GetRequestID(),
                                                 bytes_read, &defer)) {
    CancelRequestInternal(request, false);
    return false;
  }

  if (defer)
    PauseRequest(info->GetChildID(), info->GetRequestID(), true);

  return *bytes_read != 0;
}
