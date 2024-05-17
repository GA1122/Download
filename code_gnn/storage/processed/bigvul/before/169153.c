void RenderFrameHostImpl::FailedNavigation(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    bool has_stale_copy_in_cache,
    int error_code,
    const base::Optional<std::string>& error_page_content) {
  TRACE_EVENT2("navigation", "RenderFrameHostImpl::FailedNavigation",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(),
               "error", error_code);

  UpdatePermissionsForNavigation(common_params, request_params);

  ResetWaitingState();

  std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories;
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    network::mojom::URLLoaderFactoryPtrInfo default_factory_info;
    CreateNetworkServiceDefaultFactoryAndObserve(
        mojo::MakeRequest(&default_factory_info));
    subresource_loader_factories = std::make_unique<URLLoaderFactoryBundleInfo>(
        std::move(default_factory_info),
        std::map<std::string, network::mojom::URLLoaderFactoryPtrInfo>());
  }

  GetNavigationControl()->CommitFailedNavigation(
      common_params, request_params, has_stale_copy_in_cache, error_code,
      error_page_content, std::move(subresource_loader_factories));

  is_loading_ = true;
  DCHECK(GetNavigationHandle() &&
         GetNavigationHandle()->GetNetErrorCode() != net::OK);
}
