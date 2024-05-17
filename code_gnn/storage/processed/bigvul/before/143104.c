bool ServiceWorkerNetworkProvider::IsControlledByServiceWorker() const {
  return context() && context()->controller();
}
