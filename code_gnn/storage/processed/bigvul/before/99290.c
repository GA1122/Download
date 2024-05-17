void ResourceMessageFilter::OnDownloadUrl(const IPC::Message& message,
                                          const GURL& url,
                                          const GURL& referrer) {
  URLRequestContext* context = request_context_->GetURLRequestContext();
  resource_dispatcher_host_->BeginDownload(url,
                                           referrer,
                                           DownloadSaveInfo(),
                                           id(),
                                           message.routing_id(),
                                           context);
}
