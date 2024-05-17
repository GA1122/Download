void ResourceDispatcherHostImpl::ContinueSSLRequest(
    const GlobalRequestID& id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  net::URLRequest* request = GetURLRequest(id);
  if (!request)
    return;
  DVLOG(1) << "ContinueSSLRequest() url: " << request->url().spec();
  request->ContinueDespiteLastError();
}
