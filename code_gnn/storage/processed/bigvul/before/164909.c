ResourceDispatcherHostImpl::AddStandardHandlers(
    net::URLRequest* request,
    ResourceType resource_type,
    ResourceContext* resource_context,
    network::mojom::FetchRequestMode fetch_request_mode,
    blink::mojom::RequestContextType fetch_request_context_type,
    uint32_t url_loader_options,
    AppCacheService* appcache_service,
    int child_id,
    int route_id,
    std::unique_ptr<ResourceHandler> handler) {
  handler.reset(new InterceptingResourceHandler(std::move(handler), request));
  InterceptingResourceHandler* intercepting_handler =
      static_cast<InterceptingResourceHandler*>(handler.get());

  std::vector<std::unique_ptr<ResourceThrottle>> throttles;

  if (delegate_) {
    delegate_->RequestBeginning(request,
                                resource_context,
                                appcache_service,
                                resource_type,
                                &throttles);
  }

  std::unique_ptr<ResourceThrottle> clear_site_data_throttle =
      ClearSiteDataThrottle::MaybeCreateThrottleForRequest(request);
  if (clear_site_data_throttle)
    throttles.push_back(std::move(clear_site_data_throttle));

  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  throttles.push_back(std::make_unique<ScheduledResourceRequestAdapter>(
      scheduler_->ScheduleRequest(child_id, route_id, info->IsAsync(),
                                  request)));

  std::vector<std::unique_ptr<ResourceThrottle>> pre_mime_sniffing_throttles;
  std::vector<std::unique_ptr<ResourceThrottle>> post_mime_sniffing_throttles;
  for (auto& throttle : throttles) {
    if (throttle->MustProcessResponseBeforeReadingBody()) {
      pre_mime_sniffing_throttles.push_back(std::move(throttle));
    } else {
      post_mime_sniffing_throttles.push_back(std::move(throttle));
    }
  }
  throttles.clear();

  handler.reset(new ThrottlingResourceHandler(
      std::move(handler), request, std::move(post_mime_sniffing_throttles)));

  PluginService* plugin_service = nullptr;
#if BUILDFLAG(ENABLE_PLUGINS)
  plugin_service = PluginService::GetInstance();
#endif

  if (!IsResourceTypeFrame(resource_type)) {
    handler.reset(new CrossSiteDocumentResourceHandler(
        std::move(handler), request, fetch_request_mode));
  }

  if (url_loader_options & network::mojom::kURLLoadOptionSniffMimeType) {
    handler.reset(new MimeSniffingResourceHandler(
        std::move(handler), this, plugin_service, intercepting_handler, request,
        fetch_request_context_type));
  }

  handler.reset(new ThrottlingResourceHandler(
      std::move(handler), request, std::move(pre_mime_sniffing_throttles)));

  return handler;
}
