void ServiceWorkerContextCore::RegistrationComplete(
    const GURL& scope,
    ServiceWorkerContextCore::RegistrationCallback callback,
    blink::ServiceWorkerStatusCode status,
    const std::string& status_message,
    ServiceWorkerRegistration* registration) {
  if (status != blink::ServiceWorkerStatusCode::kOk) {
    DCHECK(!registration);
    std::move(callback).Run(status, status_message,
                            blink::mojom::kInvalidServiceWorkerRegistrationId);
    return;
  }

  DCHECK(registration);
  std::move(callback).Run(status, status_message, registration->id());
  observer_list_->Notify(
      FROM_HERE, &ServiceWorkerContextCoreObserver::OnRegistrationCompleted,
      registration->id(), scope);
}
