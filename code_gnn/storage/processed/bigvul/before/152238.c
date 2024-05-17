RenderFrameImpl::CreateURLLoaderFactory() {
  if (!RenderThreadImpl::current()) {
    return WebURLLoaderFactoryImpl::CreateTestOnlyFactory();
  }
  return std::make_unique<FrameURLLoaderFactory>(weak_factory_.GetWeakPtr());
}
