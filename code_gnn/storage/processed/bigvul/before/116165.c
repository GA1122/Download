void ResourceDispatcherHostImpl::ResumeRequest(
    const GlobalRequestID& request_id) {
  PendingRequestList::iterator i = pending_requests_.find(request_id);
  if (i == pending_requests_.end())   
    return;

  net::URLRequest* request = i->second;
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);

  if (!info->is_paused())
    return;
  info->set_is_paused(false);
  if (PauseRequestIfNeeded(info))
    return;

  VLOG(1) << "Resuming: \"" << i->second->url().spec() << "\""
          << " paused_read_bytes = " << info->paused_read_bytes()
          << " called response started = " << info->called_on_response_started()
          << " started reading = " << info->has_started_reading();

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
