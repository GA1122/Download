FrameFetchContext::IsControlledByServiceWorker() const {
  if (IsDetached())
    return blink::mojom::ControllerServiceWorkerMode::kNoController;

  DCHECK(MasterDocumentLoader());

  auto* service_worker_network_provider =
      MasterDocumentLoader()->GetServiceWorkerNetworkProvider();
  if (!service_worker_network_provider)
    return blink::mojom::ControllerServiceWorkerMode::kNoController;
  return service_worker_network_provider->IsControlledByServiceWorker();
}
