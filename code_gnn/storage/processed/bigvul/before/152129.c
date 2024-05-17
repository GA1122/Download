void ServiceWorkerContextCore::DidGetRegistrationsForDeleteForOrigin(
    base::OnceCallback<void(blink::ServiceWorkerStatusCode)> callback,
    blink::ServiceWorkerStatusCode status,
    const std::vector<scoped_refptr<ServiceWorkerRegistration>>&
        registrations) {
  if (status != blink::ServiceWorkerStatusCode::kOk) {
    std::move(callback).Run(status);
    return;
  }
  if (registrations.empty()) {
    std::move(callback).Run(blink::ServiceWorkerStatusCode::kOk);
    return;
  }

  int* expected_calls = new int(2 * registrations.size());
  base::RepeatingCallback<void(blink::ServiceWorkerStatusCode)> barrier =
      base::BindRepeating(SuccessReportingCallback, base::Owned(expected_calls),
                          base::AdaptCallbackForRepeating(std::move(callback)));
  for (const auto& registration : registrations) {
    DCHECK(registration);
    if (!registration->is_deleted()) {
      RegistrationDeletionListener::WaitForDeletion(
          registration,
          base::BindOnce(barrier, blink::ServiceWorkerStatusCode::kOk));
    } else {
      barrier.Run(blink::ServiceWorkerStatusCode::kOk);
    }
    job_coordinator_->Abort(registration->scope());
    UnregisterServiceWorker(registration->scope(), barrier);
  }
}
