ResourceDispatcherHostImpl::CreateResourceHandler(
    ResourceRequesterInfo* requester_info,
    net::URLRequest* request,
    const network::ResourceRequest& request_data,
    int route_id,
    int child_id,
    ResourceContext* resource_context,
    uint32_t url_loader_options,
    network::mojom::URLLoaderRequest mojo_request,
    network::mojom::URLLoaderClientPtr url_loader_client) {
  DCHECK(requester_info->IsRenderer() ||
         requester_info->IsNavigationPreload() ||
         requester_info->IsCertificateFetcherForSignedExchange());
  std::unique_ptr<ResourceHandler> handler =
      std::make_unique<MojoAsyncResourceHandler>(
          request, this, std::move(mojo_request), std::move(url_loader_client),
          static_cast<ResourceType>(request_data.resource_type),
          url_loader_options);

  if (request_data.resource_type == RESOURCE_TYPE_PREFETCH) {
    auto detachable_handler = std::make_unique<DetachableResourceHandler>(
        request,
        base::TimeDelta::FromMilliseconds(kDefaultDetachableCancelDelayMs),
        std::move(handler));
    handler = std::move(detachable_handler);
  }

  return AddStandardHandlers(
      request, static_cast<ResourceType>(request_data.resource_type),
      resource_context, request_data.fetch_request_mode,
      static_cast<blink::mojom::RequestContextType>(
          request_data.fetch_request_context_type),
      url_loader_options, requester_info->appcache_service(), child_id,
      route_id, std::move(handler));
}
