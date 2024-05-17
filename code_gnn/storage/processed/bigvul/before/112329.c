void ResourceDispatcherHostImpl::DidReceiveResponse(ResourceLoader* loader) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();

  int render_process_id, render_view_id;
  if (!info->GetAssociatedRenderView(&render_process_id, &render_view_id))
    return;

  scoped_ptr<ResourceRequestDetails> detail(new ResourceRequestDetails(
      loader->request(),
      GetCertID(loader->request(), info->GetChildID())));
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(
          &NotifyOnUI<ResourceRequestDetails>,
          static_cast<int>(NOTIFICATION_RESOURCE_RESPONSE_STARTED),
          render_process_id, render_view_id, base::Passed(&detail)));
}
