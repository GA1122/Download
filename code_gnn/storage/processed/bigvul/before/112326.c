ResourceDispatcherHostImpl::CreateResourceHandlerForDownload(
    net::URLRequest* request,
    bool is_content_initiated,
    const DownloadSaveInfo& save_info,
    const DownloadResourceHandler::OnStartedCallback& started_cb) {
  scoped_ptr<ResourceHandler> handler(
      new DownloadResourceHandler(request, started_cb, save_info));
  if (delegate_) {
    const ResourceRequestInfo* request_info(
        ResourceRequestInfo::ForRequest(request));

    ScopedVector<ResourceThrottle> throttles;
    delegate_->DownloadStarting(
        request, request_info->GetContext(), request_info->GetChildID(),
        request_info->GetRouteID(), request_info->GetRequestID(),
        is_content_initiated, &throttles);
    if (!throttles.empty()) {
      handler.reset(
          new ThrottlingResourceHandler(
              handler.Pass(), request_info->GetChildID(),
              request_info->GetRequestID(), throttles.Pass()));
    }
  }
  return handler.Pass();
}
