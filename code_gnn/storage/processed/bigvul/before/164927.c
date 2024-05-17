void ResourceDispatcherHostImpl::DidReceiveResponse(
    ResourceLoader* loader,
    network::ResourceResponse* response) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();
  net::URLRequest* request = loader->request();
  if (delegate_)
    delegate_->OnResponseStarted(request, info->GetContext(), response);
}
