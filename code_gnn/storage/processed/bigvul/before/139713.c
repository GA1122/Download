 void ResourceFetcher::ClearPreloads(ClearPreloadsPolicy policy) {
  if (!preloads_)
    return;

  LogPreloadStats(policy);

  for (const auto& resource : *preloads_) {
    if (policy == kClearAllPreloads || !resource->IsLinkPreload()) {
      resource->DecreasePreloadCount();
      if (resource->GetPreloadResult() == Resource::kPreloadNotReferenced)
        GetMemoryCache()->Remove(resource.Get());
      preloads_->erase(resource);
    }
  }
  if (!preloads_->size())
    preloads_.Clear();
}
