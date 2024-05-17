void ResourceDispatcherHostImpl::OnUploadProgressACK(int request_id) {
  ResourceLoader* loader = GetLoader(filter_->child_id(), request_id);
  if (loader)
    loader->OnUploadProgressACK();
}
