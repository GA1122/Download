scoped_refptr<SharedURLLoaderFactory> RenderFrameImpl::GetURLLoaderFactory() {
  return GetLoaderFactoryBundle();
}
