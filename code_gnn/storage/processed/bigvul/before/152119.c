void ServiceWorkerContextCore::AddProviderHost(
    std::unique_ptr<ServiceWorkerProviderHost> host) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  int provider_id = host->provider_id();
  providers_->emplace(provider_id, std::move(host));
}
