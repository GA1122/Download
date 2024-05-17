void RenderFrameImpl::UpdateSubresourceLoaderFactories(
    std::unique_ptr<blink::URLLoaderFactoryBundleInfo>
        subresource_loader_factories) {
  DCHECK(loader_factories_);
  if (loader_factories_->IsHostChildURLLoaderFactoryBundle()) {
    static_cast<HostChildURLLoaderFactoryBundle*>(loader_factories_.get())
        ->UpdateThisAndAllClones(std::move(subresource_loader_factories));
  } else {
#if DCHECK_IS_ON()
    if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      WebURL url = GetWebFrame()->GetDocument().Url();
      if (url.IsValid() && !url.IsEmpty())
        DCHECK(url.ProtocolIs(url::kAboutScheme));
    }
#endif
    auto partial_bundle = base::MakeRefCounted<ChildURLLoaderFactoryBundle>();
    static_cast<blink::URLLoaderFactoryBundle*>(partial_bundle.get())
        ->Update(std::move(subresource_loader_factories));
    loader_factories_->Update(partial_bundle->PassInterface());
  }
}
