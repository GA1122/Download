scoped_ptr<ResourceHandler> ResourceDispatcherHostImpl::AddStandardHandlers(
    net::URLRequest* request,
    ResourceType resource_type,
    ResourceContext* resource_context,
    AppCacheService* appcache_service,
    int child_id,
    int route_id,
    scoped_ptr<ResourceHandler> handler) {
  if (IsBrowserSideNavigationEnabled() && IsResourceTypeFrame(resource_type) &&
      child_id != -1) {
    DCHECK(request->url().SchemeIs(url::kBlobScheme));
    return handler;
  }

  PluginService* plugin_service = nullptr;
#if defined(ENABLE_PLUGINS)
  plugin_service = PluginService::GetInstance();
#endif
  handler.reset(new MimeTypeResourceHandler(std::move(handler), this,
                                            plugin_service, request));

  ScopedVector<ResourceThrottle> throttles;

  if (!IsBrowserSideNavigationEnabled() && IsResourceTypeFrame(resource_type))
    throttles.push_back(new NavigationResourceThrottle(request));

  if (delegate_) {
    delegate_->RequestBeginning(request,
                                resource_context,
                                appcache_service,
                                resource_type,
                                &throttles);
  }

  if (request->has_upload()) {
    throttles.push_back(
        new PowerSaveBlockResourceThrottle(request->url().host()));
  }

  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  throttles.push_back(scheduler_->ScheduleRequest(child_id, route_id,
                                                  info->IsAsync(), request));

  handler.reset(new ThrottlingResourceHandler(std::move(handler), request,
                                              std::move(throttles)));

  return handler;
}
