void TopSitesImpl::MoveStateToLoaded() {
  DCHECK(thread_checker_.CalledOnValidThread());

  MostVisitedURLList filtered_urls_all;
  MostVisitedURLList filtered_urls_nonforced;
  PendingCallbacks pending_callbacks;
  {
    base::AutoLock lock(lock_);

    if (loaded_)
      return;   
    loaded_ = true;

    if (!pending_callbacks_.empty()) {
      filtered_urls_all = thread_safe_cache_->top_sites();
      filtered_urls_nonforced.assign(thread_safe_cache_->top_sites().begin() +
                                       thread_safe_cache_->GetNumForcedURLs(),
                                     thread_safe_cache_->top_sites().end());
      pending_callbacks.swap(pending_callbacks_);
    }
  }

  for (size_t i = 0; i < pending_callbacks.size(); i++)
    pending_callbacks[i].Run(filtered_urls_all, filtered_urls_nonforced);

  if (history_service_)
    history_service_observer_.Add(history_service_);

  NotifyTopSitesLoaded();
}
