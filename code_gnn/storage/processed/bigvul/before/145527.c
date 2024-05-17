void ResourceDispatcherHostImpl::ResumeDeferredNavigation(
    const GlobalRequestID& id) {
  ResourceLoader* loader = GetLoader(id);
  if (loader)
    loader->CompleteTransfer();
}
