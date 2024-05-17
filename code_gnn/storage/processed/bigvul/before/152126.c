void ServiceWorkerContextCore::DeleteForOrigin(const GURL& origin,
                                               StatusCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  storage()->GetRegistrationsForOrigin(
      origin,
      base::BindOnce(
          &ServiceWorkerContextCore::DidGetRegistrationsForDeleteForOrigin,
          AsWeakPtr(), std::move(callback)));
}
