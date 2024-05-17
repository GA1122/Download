void ResourceDispatcherHostImpl::NotifyReceivedRedirect(
    net::URLRequest* request,
    int child_id,
    const GURL& new_url) {
  int render_process_id, render_view_id;
  if (!RenderViewForRequest(request, &render_process_id, &render_view_id))
    return;

  ResourceRedirectDetails* detail = new ResourceRedirectDetails(
      request, GetCertID(request, child_id), new_url);
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(
          &ResourceDispatcherHostImpl::NotifyOnUI<ResourceRedirectDetails>,
          static_cast<int>(NOTIFICATION_RESOURCE_RECEIVED_REDIRECT),
          render_process_id, render_view_id, detail));
}
