void ResourceDispatcherHostImpl::DidReceiveRedirect(ResourceLoader* loader,
                                                    const GURL& new_url) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();

  int render_process_id, render_frame_host;
  if (!info->GetAssociatedRenderFrame(&render_process_id, &render_frame_host))
    return;

  net::URLRequest* request = loader->request();
  if (request->response_info().async_revalidation_required) {
    DCHECK_EQ(request->url_chain().size(), 1u);
    DCHECK(async_revalidation_manager_);

    async_revalidation_manager_->BeginAsyncRevalidation(*request,
                                                        scheduler_.get());
  }

  if (request->load_flags() & net::LOAD_SUPPORT_ASYNC_REVALIDATION) {
    int new_load_flags =
        request->load_flags() & ~net::LOAD_SUPPORT_ASYNC_REVALIDATION;
    request->SetLoadFlags(new_load_flags);
  }

  if (info->IsDownload())
    return;

  scoped_ptr<ResourceRedirectDetails> detail(new ResourceRedirectDetails(
      loader->request(),
      GetCertID(loader->request(), info->GetChildID()),
      new_url));
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(
          &NotifyRedirectOnUI,
          render_process_id, render_frame_host, base::Passed(&detail)));
}
