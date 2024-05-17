void ResourceDispatcherHostImpl::RunAuthRequiredCallback(
    GlobalRequestID request_id,
    const base::Optional<net::AuthCredentials>& credentials) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  ResourceLoader* loader = GetLoader(request_id);
  if (!loader)
    return;

  net::URLRequest* url_request = loader->request();
  if (!url_request)
    return;

  if (!credentials.has_value()) {
    url_request->CancelAuth();
  } else {
    url_request->SetAuth(credentials.value());
  }

  loader->ClearLoginDelegate();
}
