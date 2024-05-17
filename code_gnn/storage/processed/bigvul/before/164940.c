bool ResourceDispatcherHostImpl::IsRequestIDInUse(
    const GlobalRequestID& id) const {
  if (pending_loaders_.find(id) != pending_loaders_.end())
    return true;
  for (const auto& blocked_loaders : blocked_loaders_map_) {
    for (const auto& loader : *blocked_loaders.second.get()) {
      ResourceRequestInfoImpl* info = loader->GetRequestInfo();
      if (info->GetGlobalRequestID() == id)
        return true;
    }
  }
  return false;
}
