void RenderFrameHostImpl::OnNetworkServiceConnectionError() {
  DCHECK(base::FeatureList::IsEnabled(network::features::kNetworkService));
  DCHECK(network_service_connection_error_handler_holder_.is_bound() &&
         network_service_connection_error_handler_holder_.encountered_error());
  network::mojom::URLLoaderFactoryPtrInfo default_factory_info;
  CreateNetworkServiceDefaultFactoryAndObserve(
      mojo::MakeRequest(&default_factory_info));

  std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loader_factories =
      std::make_unique<URLLoaderFactoryBundleInfo>();
  subresource_loader_factories->default_factory_info() =
      std::move(default_factory_info);
  GetNavigationControl()->UpdateSubresourceLoaderFactories(
      std::move(subresource_loader_factories));
}
