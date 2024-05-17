void ServiceWorkerContextCore::OnStorageWiped() {
  observer_list_->Notify(FROM_HERE,
                         &ServiceWorkerContextCoreObserver::OnStorageWiped);
}
