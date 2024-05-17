void ResourceDispatcherHostImpl::StartLoading(
    ResourceRequestInfoImpl* info,
    const linked_ptr<ResourceLoader>& loader) {
  pending_loaders_[info->GetGlobalRequestID()] = loader;

  loader->StartRequest();
}
