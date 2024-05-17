void RenderProcessHostImpl::CreateURLLoaderFactory(
    network::mojom::URLLoaderFactoryRequest request) {
  if (!base::FeatureList::IsEnabled(features::kNetworkService)) {
    NOTREACHED();
    return;
  }
  storage_partition_impl_->GetNetworkContext()->CreateURLLoaderFactory(
      std::move(request), id_);
}
