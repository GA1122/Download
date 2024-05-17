void ServiceWorkerContextCore::UnregisterServiceWorker(
    const GURL& scope,
    UnregistrationCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  job_coordinator_->Unregister(
      scope, base::BindOnce(&ServiceWorkerContextCore::UnregistrationComplete,
                            AsWeakPtr(), scope, std::move(callback)));
}
