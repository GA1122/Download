void ServiceWorkerContextCore::CheckHasServiceWorker(
    const GURL& url,
    ServiceWorkerContext::CheckHasServiceWorkerCallback callback) {
  storage()->FindRegistrationForDocument(
      url, base::BindOnce(&ServiceWorkerContextCore::
                              DidFindRegistrationForCheckHasServiceWorker,
                          AsWeakPtr(), std::move(callback)));
}
