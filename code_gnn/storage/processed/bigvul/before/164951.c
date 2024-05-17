void ResourceDispatcherHostImpl::OnRequestResourceInternal(
    ResourceRequesterInfo* requester_info,
    int routing_id,
    int request_id,
    bool is_sync_load,
    const network::ResourceRequest& request_data,
    uint32_t url_loader_options,
    network::mojom::URLLoaderRequest mojo_request,
    network::mojom::URLLoaderClientPtr url_loader_client,
    const net::NetworkTrafficAnnotationTag& traffic_annotation) {
  DCHECK(requester_info->IsRenderer() ||
         requester_info->IsNavigationPreload() ||
         requester_info->IsCertificateFetcherForSignedExchange());
  BeginRequest(requester_info, request_id, request_data, is_sync_load,
               routing_id, url_loader_options, std::move(mojo_request),
               std::move(url_loader_client), traffic_annotation);
}
