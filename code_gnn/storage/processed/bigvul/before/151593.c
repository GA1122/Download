void ResourceLoader::ActivateCacheAwareLoadingIfNeeded(
    const ResourceRequest& request) {
  DCHECK(!is_cache_aware_loading_activated_);

  if (resource_->Options().cache_aware_loading_enabled !=
      kIsCacheAwareLoadingEnabled)
    return;

  if (resource_->Options().synchronous_policy == kRequestSynchronously)
    return;

  if (resource_->IsCacheValidator())
    return;

  if (request.GetCachePolicy() != WebCachePolicy::kUseProtocolCachePolicy)
    return;

  is_cache_aware_loading_activated_ = true;
}
