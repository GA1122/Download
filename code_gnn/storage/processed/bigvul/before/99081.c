URLRequest* ResourceDispatcherHost::GetURLRequest(
    const GlobalRequestID& request_id) const {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  PendingRequestList::const_iterator i = pending_requests_.find(request_id);
  if (i == pending_requests_.end())
    return NULL;

  return i->second;
}
