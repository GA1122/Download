void ResourceDispatcherHostImpl::ClearSSLClientAuthHandlerForRequest(
    net::URLRequest* request) {
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  if (info) {
    ResourceLoader* loader = GetLoader(info->GetGlobalRequestID());
    if (loader)
      loader->ClearSSLClientAuthHandler();
  }
}
