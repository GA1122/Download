ResourceDispatcherHostImpl::CreateResourceHandlerForDownload(
    net::URLRequest* request,
    ResourceContext* context,
    int child_id,
    int route_id,
    int request_id,
    bool is_content_initiated,
    const DownloadSaveInfo& save_info,
    const DownloadResourceHandler::OnStartedCallback& started_cb) {
  scoped_refptr<ResourceHandler> handler(
      new DownloadResourceHandler(child_id, route_id, request_id,
                                  request->url(), download_file_manager_.get(),
                                  request, started_cb, save_info));
  if (delegate_) {
    ScopedVector<ResourceThrottle> throttles;
    delegate_->DownloadStarting(request, context, child_id, route_id,
                                request_id, is_content_initiated, &throttles);
    if (!throttles.empty()) {
      handler = new ThrottlingResourceHandler(this, handler, child_id,
                                              request_id, throttles.Pass());
    }
  }
  return handler;
}
