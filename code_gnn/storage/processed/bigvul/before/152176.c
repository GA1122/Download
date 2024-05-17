void ServiceWorkerContextCore::UpdateServiceWorker(
    ServiceWorkerRegistration* registration,
    bool force_bypass_cache) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  job_coordinator_->Update(registration, force_bypass_cache);
}
