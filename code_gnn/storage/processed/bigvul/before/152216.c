void RenderFrameImpl::CommitFailedPerNavigationMojoInterfaceNavigation(
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    bool has_stale_copy_in_cache,
    int error_code,
    const base::Optional<std::string>& error_page_content,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories,
    mojom::NavigationClient::CommitFailedNavigationCallback
        per_navigation_mojo_interface_callback) {
  DCHECK(navigation_client_impl_);
  DCHECK(IsPerNavigationMojoInterfaceEnabled());
  CommitFailedNavigationInternal(
      common_params, commit_params, has_stale_copy_in_cache, error_code,
      error_page_content, std::move(subresource_loader_factories),
      mojom::FrameNavigationControl::CommitFailedNavigationCallback(),
      std::move(per_navigation_mojo_interface_callback));
}
