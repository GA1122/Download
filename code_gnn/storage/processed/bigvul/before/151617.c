ResourceLoader::ResourceLoader(ResourceFetcher* fetcher,
                               ResourceLoadScheduler* scheduler,
                               Resource* resource)
    : scheduler_client_id_(ResourceLoadScheduler::kInvalidClientId),
      fetcher_(fetcher),
      scheduler_(scheduler),
      resource_(resource),
      is_cache_aware_loading_activated_(false),
      cancel_timer_(
          GetTaskRunnerFor(resource_->GetResourceRequest(), Context()),
          this,
          &ResourceLoader::CancelTimerFired) {
  DCHECK(resource_);
  DCHECK(fetcher_);

  resource_->SetLoader(this);
}
