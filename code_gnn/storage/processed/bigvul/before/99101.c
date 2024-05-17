void ResourceDispatcherHost::OnReadCompleted(URLRequest* request,
                                             int bytes_read) {
  DCHECK(request);
  RESOURCE_LOG("OnReadCompleted: " << request->url().spec());
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
  if (PauseRequestIfNeeded(info)) {
    info->set_paused_read_bytes(bytes_read);
    RESOURCE_LOG("OnReadCompleted pausing: " << request->url().spec());
    return;
  }

  if (request->status().is_success() && CompleteRead(request, &bytes_read)) {
    if (info->pause_count() == 0 &&
        Read(request, &bytes_read) &&
        request->status().is_success()) {
      if (bytes_read == 0) {
        CompleteRead(request, &bytes_read);
      } else {
        info->set_paused_read_bytes(bytes_read);
        info->set_is_paused(true);
        GlobalRequestID id(info->child_id(), info->request_id());
        MessageLoop::current()->PostTask(
            FROM_HERE,
            method_runner_.NewRunnableMethod(
                &ResourceDispatcherHost::ResumeRequest, id));
        return;
      }
    }
  }

  if (PauseRequestIfNeeded(info)) {
    info->set_paused_read_bytes(bytes_read);
    RESOURCE_LOG("OnReadCompleted (CompleteRead) pausing: " <<
                 request->url().spec());
    return;
  }

  if (!request->status().is_io_pending())
    OnResponseCompleted(request);
}
