void RenderFrameHostImpl::FailedNavigation(
    int64_t navigation_id,
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

  url::Origin origin = url::Origin();

  std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories;
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    network::mojom::URLLoaderFactoryPtrInfo default_factory_info;
    bool bypass_redirect_checks = CreateNetworkServiceDefaultFactoryAndObserve(
        origin, mojo::MakeRequest(&default_factory_info));
    subresource_loader_factories = std::make_unique<URLLoaderFactoryBundleInfo>(
        std::move(default_factory_info),
        URLLoaderFactoryBundleInfo::SchemeMap(),
        URLLoaderFactoryBundleInfo::OriginMap(), bypass_redirect_checks);
  }

  auto find_request = navigation_requests_.find(navigation_id);
  NavigationRequest* request = find_request != navigation_requests_.end()
                                   ? find_request->second.get()
                                   : nullptr;
  if (IsPerNavigationMojoInterfaceEnabled() && request &&
      request->GetCommitNavigationClient()) {
    request->GetCommitNavigationClient()->CommitFailedNavigation(
        common_params, request_params, has_stale_copy_in_cache, error_code,
        error_page_content, std::move(subresource_loader_factories),
        base::BindOnce(&RenderFrameHostImpl::OnCrossDocumentCommitProcessed,
                       base::Unretained(this), navigation_id));
  } else {
    GetNavigationControl()->CommitFailedNavigation(
        common_params, request_params, has_stale_copy_in_cache, error_code,
        error_page_content, std::move(subresource_loader_factories),
        base::BindOnce(&RenderFrameHostImpl::OnCrossDocumentCommitProcessed,
                       base::Unretained(this), navigation_id));
  }

  is_loading_ = true;
  DCHECK(request && request->navigation_handle() &&
         request->navigation_handle()->GetNetErrorCode() != net::OK);
}
