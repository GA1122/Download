void RenderFrameImpl::UpdateSubresourceLoaderFactories(
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loaders) {
  DCHECK(loader_factories_);
  DCHECK(loader_factories_->IsHostChildURLLoaderFactoryBundle());
  static_cast<HostChildURLLoaderFactoryBundle*>(loader_factories_.get())
      ->UpdateThisAndAllClones(std::move(subresource_loaders));
}
