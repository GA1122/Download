void ResourceDispatcherHostImpl::DidStartRequest(ResourceLoader* loader) {
  MaybeStartUpdateLoadInfoTimer();
}
