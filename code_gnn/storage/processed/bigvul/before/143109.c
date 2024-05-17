ServiceWorkerNetworkProvider::~ServiceWorkerNetworkProvider() {
  if (provider_id_ == kInvalidServiceWorkerProviderId)
    return;
  if (!ChildThreadImpl::current())
    return;   
  ChildThreadImpl::current()->Send(
      new ServiceWorkerHostMsg_ProviderDestroyed(provider_id_));
}
