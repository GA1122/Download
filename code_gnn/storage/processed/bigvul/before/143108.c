void ServiceWorkerNetworkProvider::SetServiceWorkerVersionId(
    int64_t version_id) {
  DCHECK_NE(kInvalidServiceWorkerProviderId, provider_id_);
  if (!ChildThreadImpl::current())
    return;   
  ChildThreadImpl::current()->Send(
      new ServiceWorkerHostMsg_SetVersionId(provider_id_, version_id));
}
