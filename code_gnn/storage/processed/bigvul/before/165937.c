void RenderFrameImpl::SetupLoaderFactoryBundle(
    std::unique_ptr<URLLoaderFactoryBundleInfo> info,
    base::Optional<std::vector<mojom::TransferrableURLLoaderPtr>>
        subresource_overrides,
    network::mojom::URLLoaderFactoryPtr prefetch_loader_factory) {
  RenderThreadImpl* render_thread = RenderThreadImpl::current();

  loader_factories_ = base::MakeRefCounted<HostChildURLLoaderFactoryBundle>(
      GetTaskRunner(blink::TaskType::kInternalLoading));

  if (render_thread && !info) {
    DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService) ||
           !frame_->GetDocumentLoader());
    loader_factories_->Update(render_thread->blink_platform_impl()
                                  ->CreateDefaultURLLoaderFactoryBundle()
                                  ->PassInterface(),
                              base::nullopt);
  }

  if (info) {
    loader_factories_->Update(
        std::make_unique<ChildURLLoaderFactoryBundleInfo>(std::move(info)),
        std::move(subresource_overrides));
  }
  if (prefetch_loader_factory) {
    loader_factories_->SetPrefetchLoaderFactory(
        std::move(prefetch_loader_factory));
  }
}
