void RenderFrameHostImpl::SendCommitNavigation(
    mojom::NavigationClient* navigation_client,
    NavigationRequest* navigation_request,
    const network::ResourceResponseHead& head,
    const content::CommonNavigationParams& common_params,
    const content::CommitNavigationParams& commit_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories,
    base::Optional<std::vector<::content::mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    blink::mojom::ControllerServiceWorkerInfoPtr controller,
    blink::mojom::ServiceWorkerProviderInfoForWindowPtr provider_info,
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory,
    const base::UnguessableToken& devtools_navigation_token) {
  if (navigation_client) {
    navigation_client->CommitNavigation(
        head, common_params, commit_params,
        std::move(url_loader_client_endpoints),
        std::move(subresource_loader_factories),
        std::move(subresource_overrides), std::move(controller),
        std::move(provider_info), std::move(prefetch_loader_factory),
        devtools_navigation_token,
        BuildNavigationClientCommitNavigationCallback(navigation_request));
  } else {
    GetNavigationControl()->CommitNavigation(
        head, common_params, commit_params,
        std::move(url_loader_client_endpoints),
        std::move(subresource_loader_factories),
        std::move(subresource_overrides), std::move(controller),
        std::move(provider_info), std::move(prefetch_loader_factory),
        devtools_navigation_token,
        BuildCommitNavigationCallback(navigation_request));
  }
}
