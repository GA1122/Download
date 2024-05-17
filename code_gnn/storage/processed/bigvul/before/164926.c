void ResourceDispatcherHostImpl::DidReceiveRedirect(
    ResourceLoader* loader,
    const GURL& new_url,
    network::ResourceResponse* response) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();
  if (delegate_) {
    delegate_->OnRequestRedirected(
        new_url, loader->request(), info->GetContext(), response);
  }
}
