void ServiceWorkerContextCore::UpdateServiceWorker(
    ServiceWorkerRegistration* registration,
    bool force_bypass_cache,
    bool skip_script_comparison,
    UpdateCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  job_coordinator_->Update(
      registration, force_bypass_cache, skip_script_comparison,
      base::BindOnce(&ServiceWorkerContextCore::UpdateComplete, AsWeakPtr(),
                     std::move(callback)));
}
