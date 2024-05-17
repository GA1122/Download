void ResourceDispatcherHost::ResumeRequest(const GlobalRequestID& request_id) {
  PendingRequestList::iterator i = pending_requests_.find(request_id);
  if (i == pending_requests_.end())   
    return;

  URLRequest* request = i->second;
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
  if (!info->is_paused())
    return;

  RESOURCE_LOG("Resuming: " << i->second->url().spec());

  info->set_is_paused(false);

  if (info->called_on_response_started()) {
    if (info->has_started_reading()) {
      OnReadCompleted(i->second, info->paused_read_bytes());
    } else {
      StartReading(request);
    }
  } else {
    OnResponseStarted(i->second);
  }
}
