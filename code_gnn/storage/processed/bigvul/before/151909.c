bool RenderFrameHostImpl::CreateNetworkServiceDefaultFactoryAndObserve(
    const base::Optional<url::Origin>& origin,
    network::mojom::URLLoaderFactoryRequest default_factory_request) {
  bool bypass_redirect_checks = CreateNetworkServiceDefaultFactoryInternal(
      origin, std::move(default_factory_request));

  if (IsOutOfProcessNetworkService() &&
      (!network_service_connection_error_handler_holder_ ||
       network_service_connection_error_handler_holder_.encountered_error())) {
    StoragePartition* storage_partition = BrowserContext::GetStoragePartition(
        GetSiteInstance()->GetBrowserContext(), GetSiteInstance());
    network::mojom::URLLoaderFactoryParamsPtr params =
        network::mojom::URLLoaderFactoryParams::New();
    params->process_id = GetProcess()->GetID();
    storage_partition->GetNetworkContext()->CreateURLLoaderFactory(
        mojo::MakeRequest(&network_service_connection_error_handler_holder_),
        std::move(params));
    network_service_connection_error_handler_holder_
        .set_connection_error_handler(base::BindOnce(
            &RenderFrameHostImpl::UpdateSubresourceLoaderFactories,
            weak_ptr_factory_.GetWeakPtr()));
  }
  return bypass_redirect_checks;
}
