void ResourceLoader::StartWith(const ResourceRequest& request) {
  DCHECK_NE(ResourceLoadScheduler::kInvalidClientId, scheduler_client_id_);
  DCHECK(loader_);

  if (resource_->Options().synchronous_policy == kRequestSynchronously &&
      Context().DefersLoading()) {
    Cancel();
    return;
  }

  loader_->SetDefersLoading(Context().DefersLoading());

  if (is_cache_aware_loading_activated_) {
    ResourceRequest cache_aware_request(request);
    cache_aware_request.SetCachePolicy(WebCachePolicy::kReturnCacheDataIfValid);
    loader_->LoadAsynchronously(WrappedResourceRequest(cache_aware_request),
                                this);
    return;
  }

  if (resource_->Options().synchronous_policy == kRequestSynchronously)
    RequestSynchronously(request);
  else
    loader_->LoadAsynchronously(WrappedResourceRequest(request), this);
}
