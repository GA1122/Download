void RenderFrameHostImpl::SendCommitFailedNavigation(
    mojom::NavigationClient* navigation_client,
    NavigationRequest* navigation_request,
    const content::CommonNavigationParams& common_params,
    const content::CommitNavigationParams& commit_params,
    bool has_stale_copy_in_cache,
    int32_t error_code,
    const base::Optional<std::string>& error_page_content,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories) {
  if (navigation_client) {
    navigation_client->CommitFailedNavigation(
        common_params, commit_params, has_stale_copy_in_cache, error_code,
        error_page_content, std::move(subresource_loader_factories),
        BuildNavigationClientCommitFailedNavigationCallback(
            navigation_request));
  } else {
    GetNavigationControl()->CommitFailedNavigation(
        common_params, commit_params, has_stale_copy_in_cache, error_code,
        error_page_content, std::move(subresource_loader_factories),
        BuildCommitFailedNavigationCallback(navigation_request));
  }
}
