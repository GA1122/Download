void ResourceDispatcherHostImpl::CancelSSLRequest(
    const GlobalRequestID& id,
    int error,
    const net::SSLInfo* ssl_info) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  net::URLRequest* request = GetURLRequest(id);
  if (!request || !request->is_pending())
    return;
  DVLOG(1) << "CancelSSLRequest() url: " << request->url().spec();
  if (ssl_info)
    request->CancelWithSSLError(error, *ssl_info);
  else
    request->CancelWithError(error);
}
