ServiceWorkerContextCore::GetClientProviderHostIterator(
    const GURL& origin,
    bool include_reserved_clients) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  return base::WrapUnique(new ProviderHostIterator(
      providers_.get(), base::BindRepeating(IsSameOriginClientProviderHost,
                                            origin, include_reserved_clients)));
}
