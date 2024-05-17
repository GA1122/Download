void RenderFrameImpl::UpdateSubresourceLoaderFactories(
    std::unique_ptr<URLLoaderFactoryBundleInfo> subresource_loaders) {
  DCHECK(loader_factories_);
  static_cast<URLLoaderFactoryBundle*>(loader_factories_.get())
      ->Update(std::move(subresource_loaders));
}
