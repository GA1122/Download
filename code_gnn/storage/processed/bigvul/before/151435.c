bool FrameFetchContext::IsControlledByServiceWorker() const {
  if (IsDetached())
    return false;

  DCHECK(MasterDocumentLoader());

  if (GetSecurityOrigin() && GetSecurityOrigin()->HasSuborigin())
    return false;

  auto* service_worker_network_provider =
      MasterDocumentLoader()->GetServiceWorkerNetworkProvider();
  return service_worker_network_provider &&
         service_worker_network_provider->HasControllerServiceWorker();
}
