void RenderProcessHostImpl::GetBlobURLLoaderFactory(
    network::mojom::URLLoaderFactoryRequest request) {
  if (!base::FeatureList::IsEnabled(features::kNetworkService)) {
    NOTREACHED();
    return;
  }
  storage_partition_impl_->GetBlobURLLoaderFactory()->HandleRequest(
      std::move(request));
}
