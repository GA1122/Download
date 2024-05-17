void ServiceWorkerContextCore::OnNoControllees(ServiceWorkerVersion* version) {
  observer_list_->Notify(FROM_HERE,
                         &ServiceWorkerContextCoreObserver::OnNoControllees,
                         version->version_id(), version->scope());
}
