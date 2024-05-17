ServiceWorkerProviderHost* ServiceWorkerContextCore::GetProviderHost(
    int provider_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  auto found = providers_->find(provider_id);
  if (found == providers_->end())
    return nullptr;
  return found->second.get();
}
