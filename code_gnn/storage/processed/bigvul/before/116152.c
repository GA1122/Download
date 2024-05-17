 void ResourceDispatcherHostImpl::OnResponseStarted(net::URLRequest* request) {
  VLOG(1) << "OnResponseStarted: " << request->url().spec();
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  if (request->status().is_success()) {
    if (PauseRequestIfNeeded(info)) {
      VLOG(1) << "OnResponseStarted pausing: " << request->url().spec();
      return;
    }

    info->set_waiting_for_upload_progress_ack(false);
    MaybeUpdateUploadProgress(info, request);

    if (!CompleteResponseStarted(request)) {
      CancelRequestInternal(request, false);
    } else {
      if (PauseRequestIfNeeded(info)) {
        VLOG(1) << "OnResponseStarted pausing2: " << request->url().spec();
        return;
      }

      StartReading(request);
    }
  } else {
    ResponseCompleted(request);
  }
}
