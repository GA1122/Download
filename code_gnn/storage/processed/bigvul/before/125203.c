void RenderMessageFilter::OnDownloadUrl(const IPC::Message& message,
                                        const GURL& url,
                                        const Referrer& referrer,
                                        const string16& suggested_name) {
  scoped_ptr<DownloadSaveInfo> save_info(new DownloadSaveInfo());
  save_info->suggested_name = suggested_name;
  scoped_ptr<net::URLRequest> request(
      resource_context_->GetRequestContext()->CreateRequest(url, NULL));
  request->set_referrer(referrer.url.spec());
  webkit_glue::ConfigureURLRequestForReferrerPolicy(
      request.get(), referrer.policy);
  RecordDownloadSource(INITIATED_BY_RENDERER);
  resource_dispatcher_host_->BeginDownload(
      request.Pass(),
      true,   
      resource_context_,
      render_process_id_,
      message.routing_id(),
      false,
      save_info.Pass(),
      content::DownloadId::Invalid(),
      ResourceDispatcherHostImpl::DownloadStartedCallback());
}
