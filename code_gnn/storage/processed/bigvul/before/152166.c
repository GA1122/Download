void ServiceWorkerContextCore::RemoveProviderHost(int provider_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  providers_->erase(provider_id);
}
