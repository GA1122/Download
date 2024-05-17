void FrameFetchContext::DispatchNetworkQuiet() {
  if (WebServiceWorkerNetworkProvider* service_worker_network_provider =
          MasterDocumentLoader()->GetServiceWorkerNetworkProvider()) {
    service_worker_network_provider->DispatchNetworkQuiet();
  }
}
