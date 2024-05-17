void ResourceFetcher::ClearContext() {
  DCHECK(resources_from_previous_fetcher_.IsEmpty());
  scheduler_->Shutdown();
  ClearPreloads(ResourceFetcher::kClearAllPreloads);
  context_ = Context().Detach();

  StopFetching();

  if (!loaders_.IsEmpty() || !non_blocking_loaders_.IsEmpty()) {
    keepalive_loaders_task_handle_ =
        Context().GetLoadingTaskRunner()->PostDelayedCancellableTask(
            BLINK_FROM_HERE,
            WTF::Bind(&ResourceFetcher::StopFetchingIncludingKeepaliveLoaders,
                      WrapPersistent(this)),
            kKeepaliveLoadersTimeout);
  }
}
