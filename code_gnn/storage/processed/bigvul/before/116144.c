void ResourceDispatcherHostImpl::NotifyResponseStarted(net::URLRequest* request,
                                                       int child_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  int render_process_id, render_view_id;
  if (!RenderViewForRequest(request, &render_process_id, &render_view_id))
    return;

  ResourceRequestDetails* detail = new ResourceRequestDetails(
      request, GetCertID(request, child_id));
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(
          &ResourceDispatcherHostImpl::NotifyOnUI<ResourceRequestDetails>,
          static_cast<int>(NOTIFICATION_RESOURCE_RESPONSE_STARTED),
          render_process_id, render_view_id, detail));
}
