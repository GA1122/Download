void ResourceDispatcherHostImpl::OnRequestResource(
    int routing_id,
    int request_id,
    const ResourceHostMsg_Request& request_data) {
  tracked_objects::ScopedTracker tracking_profile(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "477117 ResourceDispatcherHostImpl::OnRequestResource"));
  if (request_data.resource_type == RESOURCE_TYPE_MAIN_FRAME &&
      request_data.transferred_request_request_id == -1 &&
      !IsBrowserSideNavigationEnabled()) {
    BrowserThread::PostTask(
        BrowserThread::UI,
        FROM_HERE,
        base::Bind(&LogResourceRequestTimeOnUI,
                   TimeTicks::Now(),
                   filter_->child_id(),
                   request_data.render_frame_id,
                   request_data.url));
  }
  BeginRequest(request_id, request_data, NULL, routing_id);
}
