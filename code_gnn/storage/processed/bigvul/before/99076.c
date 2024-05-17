bool ResourceDispatcherHost::CompleteRead(URLRequest* request,
                                          int* bytes_read) {
  if (!request->status().is_success()) {
    NOTREACHED();
    return false;
  }

  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
  if (!info->resource_handler()->OnReadCompleted(info->request_id(),
                                                 bytes_read)) {
    CancelRequest(info->child_id(), info->request_id(), false, false);
    return false;
  }

  return *bytes_read != 0;
}
