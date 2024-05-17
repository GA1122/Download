void ServiceWorkerContextCore::OnControlleeAdded(
    ServiceWorkerVersion* version,
    const std::string& client_uuid,
    const ServiceWorkerClientInfo& client_info) {
  observer_list_->Notify(
      FROM_HERE, &ServiceWorkerContextCoreObserver::OnControlleeAdded,
      version->version_id(), version->scope(), client_uuid, client_info);
}
