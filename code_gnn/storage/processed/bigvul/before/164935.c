ResourceDispatcherHostImpl::HandleDownloadStarted(
    net::URLRequest* request,
    std::unique_ptr<ResourceHandler> handler,
    bool is_content_initiated,
    bool must_download,
    bool is_new_request) {
  if (delegate()) {
    const ResourceRequestInfoImpl* request_info(
        ResourceRequestInfoImpl::ForRequest(request));
    std::vector<std::unique_ptr<ResourceThrottle>> throttles;
    delegate()->DownloadStarting(request, request_info->GetContext(),
                                 is_content_initiated, true, is_new_request,
                                 &throttles);
    if (!throttles.empty()) {
      handler.reset(new ThrottlingResourceHandler(std::move(handler), request,
                                                  std::move(throttles)));
    }
  }
  return handler;
}
