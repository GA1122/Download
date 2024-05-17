void ServiceWorkerContextCore::UnregistrationComplete(
    const GURL& scope,
    ServiceWorkerContextCore::UnregistrationCallback callback,
    int64_t registration_id,
    blink::ServiceWorkerStatusCode status) {
  std::move(callback).Run(status);
  if (status == blink::ServiceWorkerStatusCode::kOk) {
    observer_list_->Notify(
        FROM_HERE, &ServiceWorkerContextCoreObserver::OnRegistrationDeleted,
        registration_id, scope);
  }
}
