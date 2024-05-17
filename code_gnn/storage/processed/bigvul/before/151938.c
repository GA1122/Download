void RenderFrameHostImpl::FailedNavigation(
    NavigationRequest* navigation_request,
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    bool has_stale_copy_in_cache,
    int error_code,
    const base::Optional<std::string>& error_page_content) {
  TRACE_EVENT2("navigation", "RenderFrameHostImpl::FailedNavigation",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(),
               "error", error_code);

  UpdatePermissionsForNavigation(common_params, commit_params);

  ResetWaitingState();

  url::Origin origin = url::Origin();

  std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
      subresource_loader_factories;
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    network::mojom::URLLoaderFactoryPtrInfo default_factory_info;
    bool bypass_redirect_checks = CreateNetworkServiceDefaultFactoryAndObserve(
        origin, mojo::MakeRequest(&default_factory_info));
    subresource_loader_factories =
        std::make_unique<blink::URLLoaderFactoryBundleInfo>(
            std::move(default_factory_info),
            blink::URLLoaderFactoryBundleInfo::SchemeMap(),
            blink::URLLoaderFactoryBundleInfo::OriginMap(),
            bypass_redirect_checks);
  }

  mojom::NavigationClient* navigation_client = nullptr;
  if (IsPerNavigationMojoInterfaceEnabled())
    navigation_client = navigation_request->GetCommitNavigationClient();

  SendCommitFailedNavigation(
      navigation_client, navigation_request, common_params, commit_params,
      has_stale_copy_in_cache, error_code, error_page_content,
      std::move(subresource_loader_factories));

  is_loading_ = true;
  DCHECK(navigation_request && navigation_request->navigation_handle() &&
         navigation_request->navigation_handle()->GetNetErrorCode() != net::OK);
}
