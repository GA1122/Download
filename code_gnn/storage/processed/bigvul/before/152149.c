void ServiceWorkerContextCore::OnControlleeRemoved(
    ServiceWorkerVersion* version,
    const std::string& client_uuid) {
  observer_list_->Notify(FROM_HERE,
                         &ServiceWorkerContextCoreObserver::OnControlleeRemoved,
                         version->version_id(), version->scope(), client_uuid);
}
