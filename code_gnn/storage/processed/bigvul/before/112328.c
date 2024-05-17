void ResourceDispatcherHostImpl::DidReceiveRedirect(ResourceLoader* loader,
                                                    const GURL& new_url) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();

  int render_process_id, render_view_id;
  if (!info->GetAssociatedRenderView(&render_process_id, &render_view_id))
    return;

  scoped_ptr<ResourceRedirectDetails> detail(new ResourceRedirectDetails(
      loader->request(),
      GetCertID(loader->request(), info->GetChildID()),
      new_url));
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(
          &NotifyOnUI<ResourceRedirectDetails>,
          static_cast<int>(NOTIFICATION_RESOURCE_RECEIVED_REDIRECT),
          render_process_id, render_view_id, base::Passed(&detail)));
}
