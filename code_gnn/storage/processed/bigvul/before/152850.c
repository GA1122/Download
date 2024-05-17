void MetricsWebContentsObserver::ResourceLoadComplete(
    content::RenderFrameHost* render_frame_host,
    const content::GlobalRequestID& request_id,
    const content::mojom::ResourceLoadInfo& resource_load_info) {
  if (!base::FeatureList::IsEnabled(network::features::kNetworkService))
    return;

  if (!resource_load_info.url.SchemeIsHTTPOrHTTPS())
    return;

  PageLoadTracker* tracker = GetTrackerOrNullForRequest(
      request_id, render_frame_host, resource_load_info.resource_type,
      resource_load_info.load_timing_info.request_start);
  if (tracker) {
    int original_content_length = 0;
    std::unique_ptr<data_reduction_proxy::DataReductionProxyData>
        data_reduction_proxy_data;

    const content::mojom::CommonNetworkInfoPtr& network_info =
        resource_load_info.network_info;
    ExtraRequestCompleteInfo extra_request_complete_info(
        resource_load_info.url, network_info->ip_port_pair.value(),
        render_frame_host->GetFrameTreeNodeId(), resource_load_info.was_cached,
        resource_load_info.raw_body_bytes, original_content_length,
        std::move(data_reduction_proxy_data), resource_load_info.resource_type,
        resource_load_info.net_error,
        std::make_unique<net::LoadTimingInfo>(
            resource_load_info.load_timing_info));
    tracker->OnLoadedResource(extra_request_complete_info);
  }
}
