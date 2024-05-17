void ServiceWorkerContextCore::OnRunningStateChanged(
    ServiceWorkerVersion* version) {
  observer_list_->Notify(
      FROM_HERE, &ServiceWorkerContextCoreObserver::OnRunningStateChanged,
      version->version_id(), version->running_status());
}
