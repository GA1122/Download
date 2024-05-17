void ResourceDispatcherHost::OnResponseStarted(URLRequest* request) {
  RESOURCE_LOG("OnResponseStarted: " << request->url().spec());
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
  if (PauseRequestIfNeeded(info)) {
    RESOURCE_LOG("OnResponseStarted pausing: " << request->url().spec());
    return;
  }

  if (request->status().is_success()) {
    info->set_waiting_for_upload_progress_ack(false);
    MaybeUpdateUploadProgress(info, request);

    if (!CompleteResponseStarted(request)) {
      CancelRequest(info->child_id(), info->request_id(), false);
    } else {
      if (PauseRequestIfNeeded(info)) {
        RESOURCE_LOG("OnResponseStarted pausing2: " << request->url().spec());
        return;
      }

      StartReading(request);
    }
  } else {
    OnResponseCompleted(request);
  }
}
