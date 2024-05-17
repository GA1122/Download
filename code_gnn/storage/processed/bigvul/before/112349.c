void ResourceDispatcherHostImpl::OnDataReceivedACK(int request_id) {
  ResourceLoader* loader = GetLoader(filter_->child_id(), request_id);
  if (!loader)
    return;

  ResourceRequestInfoImpl* info = loader->GetRequestInfo();
  if (info->async_handler())
    info->async_handler()->OnDataReceivedACK();
}
