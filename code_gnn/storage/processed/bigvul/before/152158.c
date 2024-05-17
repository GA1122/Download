void ServiceWorkerContextCore::PerformStorageCleanup(
    base::OnceClosure callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  storage()->PerformStorageCleanup(std::move(callback));
}
