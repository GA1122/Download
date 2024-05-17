ResourceLoader* ResourceDispatcherHostImpl::GetLoader(
    const GlobalRequestID& id) const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  LoaderMap::const_iterator i = pending_loaders_.find(id);
  if (i == pending_loaders_.end())
    return NULL;

  return i->second.get();
}
