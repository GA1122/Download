void ResourceDispatcherHostImpl::StartLoading(
    ResourceRequestInfoImpl* info,
    std::unique_ptr<ResourceLoader> loader) {
  ResourceLoader* loader_ptr = loader.get();
  DCHECK(pending_loaders_[info->GetGlobalRequestID()] == nullptr);
  pending_loaders_[info->GetGlobalRequestID()] = std::move(loader);
  if (info->keepalive())
    keepalive_statistics_recorder_.OnLoadStarted(info->GetChildID());

  loader_ptr->StartRequest();
}
