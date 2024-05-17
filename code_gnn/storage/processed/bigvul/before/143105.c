ServiceWorkerNetworkProvider::ServiceWorkerNetworkProvider(
    int route_id,
    ServiceWorkerProviderType provider_type,
    int browser_provider_id,
    bool is_parent_frame_secure)
    : provider_id_(browser_provider_id) {
  if (provider_id_ == kInvalidServiceWorkerProviderId)
    return;
  if (!ChildThreadImpl::current())
    return;   
  context_ = new ServiceWorkerProviderContext(
      provider_id_, provider_type,
      ChildThreadImpl::current()->thread_safe_sender());
  ChildThreadImpl::current()->Send(new ServiceWorkerHostMsg_ProviderCreated(
      provider_id_, route_id, provider_type, is_parent_frame_secure));
}
