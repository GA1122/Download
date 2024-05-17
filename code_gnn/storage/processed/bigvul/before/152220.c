void RenderFrameImpl::CommitPerNavigationMojoInterfaceNavigation(
    const network::ResourceResponseHead& head,
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories,
    base::Optional<std::vector<mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    blink::mojom::ControllerServiceWorkerInfoPtr controller_service_worker_info,
    blink::mojom::ServiceWorkerProviderInfoForWindowPtr provider_info,
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory,
    const base::UnguessableToken& devtools_navigation_token,
    mojom::NavigationClient::CommitNavigationCallback
        per_navigation_mojo_interface_callback) {
  DCHECK(navigation_client_impl_);
  DCHECK(IsPerNavigationMojoInterfaceEnabled());
  CommitNavigationInternal(
      head, common_params, commit_params,
      std::move(url_loader_client_endpoints),
      std::move(subresource_loader_factories), std::move(subresource_overrides),
      std::move(controller_service_worker_info), std::move(provider_info),
      std::move(prefetch_loader_factory), devtools_navigation_token,
      mojom::FrameNavigationControl::CommitNavigationCallback(),
      std::move(per_navigation_mojo_interface_callback));
}
