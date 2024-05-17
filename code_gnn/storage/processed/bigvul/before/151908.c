bool RenderFrameHostImpl::CreateNetworkServiceDefaultFactory(
    network::mojom::URLLoaderFactoryRequest default_factory_request) {
  return CreateNetworkServiceDefaultFactoryInternal(
      last_committed_origin_, std::move(default_factory_request));
}
