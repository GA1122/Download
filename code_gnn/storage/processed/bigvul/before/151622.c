void ResourceLoader::Start() {
  const ResourceRequest& request = resource_->GetResourceRequest();
  ActivateCacheAwareLoadingIfNeeded(request);
  loader_ =
      Context().CreateURLLoader(request, GetTaskRunnerFor(request, Context()));

  ResourceLoadScheduler::ThrottleOption option =
      (resource_->Options().synchronous_policy == kRequestSynchronously ||
       request.GetRequestContext() == WebURLRequest::kRequestContextFetch)
          ? ResourceLoadScheduler::ThrottleOption::kCanNotBeThrottled
          : ResourceLoadScheduler::ThrottleOption::kCanBeThrottled;
  DCHECK_EQ(ResourceLoadScheduler::kInvalidClientId, scheduler_client_id_);
  scheduler_->Request(this, option, &scheduler_client_id_);
}
