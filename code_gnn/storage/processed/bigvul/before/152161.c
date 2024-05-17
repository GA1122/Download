void ServiceWorkerContextCore::RegisterServiceWorker(
    const GURL& script_url,
    const blink::mojom::ServiceWorkerRegistrationOptions& options,
    RegistrationCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::string error_message;
  if (!IsValidRegisterRequest(script_url, options.scope, &error_message)) {
    std::move(callback).Run(
        blink::ServiceWorkerStatusCode::kErrorInvalidArguments, error_message,
        blink::mojom::kInvalidServiceWorkerRegistrationId);
    return;
  }
  was_service_worker_registered_ = true;
  job_coordinator_->Register(
      script_url, options,
      base::BindOnce(&ServiceWorkerContextCore::RegistrationComplete,
                     AsWeakPtr(), options.scope, std::move(callback)));
}
