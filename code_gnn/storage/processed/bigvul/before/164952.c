void ResourceDispatcherHostImpl::OnRequestResourceWithMojo(
    ResourceRequesterInfo* requester_info,
    int32_t routing_id,
    int32_t request_id,
    uint32_t options,
    const network::ResourceRequest& request,
    network::mojom::URLLoaderRequest mojo_request,
    network::mojom::URLLoaderClientPtr url_loader_client,
    const net::NetworkTrafficAnnotationTag& traffic_annotation) {
  if (!url_loader_client) {
    VLOG(1) << "Killed renderer for null client";
    bad_message::ReceivedBadMessage(requester_info->filter(),
                                    bad_message::RDH_NULL_CLIENT);
    return;
  }
  bool is_sync_load = options & network::mojom::kURLLoadOptionSynchronous;
  OnRequestResourceInternal(requester_info, routing_id, request_id,
                            is_sync_load, request, options,
                            std::move(mojo_request),
                            std::move(url_loader_client), traffic_annotation);
}
