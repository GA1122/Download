void ServiceWorkerContextCore::NotifyRegistrationStored(int64_t registration_id,
                                                        const GURL& scope) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  observer_list_->Notify(
      FROM_HERE, &ServiceWorkerContextCoreObserver::OnRegistrationStored,
      registration_id, scope);
}
