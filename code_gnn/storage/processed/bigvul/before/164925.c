void ResourceDispatcherHostImpl::DidFinishLoading(ResourceLoader* loader) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();

  if (delegate_)
    delegate_->RequestComplete(loader->request());

  RemovePendingRequest(info->GetChildID(), info->GetRequestID());
}
