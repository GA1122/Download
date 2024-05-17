void StopServiceWorkerOnIO(scoped_refptr<ServiceWorkerContextWrapper> context,
                           int64_t version_id) {
  if (content::ServiceWorkerVersion* version =
          context->GetLiveVersion(version_id)) {
    version->StopWorker(base::BindOnce(&base::DoNothing));
  }
}
